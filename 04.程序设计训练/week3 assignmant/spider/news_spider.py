from requests_html import HTMLSession
from urllib.parse import urlparse
import json
import re

session = HTMLSession()
ua = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/76.0.3809.132 Safari/537.36"

def is_inner_link(link):
    netloc = urlparse(link).netloc
    return (netloc == 'sports.sina.com.cn')

links = set() #需要爬取的网页集
#BFS用
visited = []
unvisited = ["http://sports.sina.com.cn/",
             "http://sports.sina.com.cn/2019worldcup/",
             "http://sports.sina.com.cn/nba/",
             "http://sports.sina.com.cn/nba/3.shtml"
             ]

def add_unvisited(link):
    if not is_inner_link(link):
        return
    if (link in visited) or (link in unvisited):
        return  
    path = urlparse(link).path

    if not path.startswith(('/basketball','/zl/basketball','/nba')) or (path.find("video") != -1):
        return
    if re.search(r'\/nba',path) and not re.search(r'\/nba\/\d{0,2}\.shtml',path):
        if not re.search(r'\/nba\/201(8|9)',path):
            return

    if link.startswith('//'):
        link = 'http:'+link

    unvisited.append(link)
    if path.find("doc")!=-1:
        links.add(link)

def initLink():
    while len(unvisited):
        if(len(links)>5000): 
            break
        link = unvisited.pop()
        r = session.get(link,headers={'user-agent':ua})
        visited.append(link)

        if r.html and r.html.links and len(r.html.links):
            for url in r.html.links:
                add_unvisited(url)
        print(link,len(links))


def get_page(link,all):#参数:网页链接，存储字典
    r = session.get(link,headers={'user-agent':ua})
    title = r.html.find('body > div.main-content.w1240 > h1',first=True).text
    stamp = r.html.find('#top_bar > div > div.date-source > span',first=True).text
    source = r.html.find('#top_bar > div > div.date-source > a',first=True)
    source_text = source.text
    source_link = ''
    if(len(source.links)):
        source_link = list(source.links)[0]
    def escape(x):
        x = x.replace(u'\xa0', u' ')
        x = x.replace(u'\u3000', u' ')
        x = x.strip()
        x = re.sub(r'<script>[\s\S]*<\/script>','',x)
        return x
    content = r.html.find('#artibody',first=True).html
    content = escape(content)
    all['count']+=1
    all['data']['xinlang_{}'.format(all['count'])]={
        'title':title,
        'stamp':stamp,
        'source':{
            'link':source_link,
            'text':source_text
            },
        'content':content,
        'url':link
        }
    print('collect No.{}: {}'.format(all['count'], link))

def main():
    print('============ starting  ============')
    f = None
    all = dict()
    try:
        f = open('nba.json', 'r')
        all = json.loads(f.read())
        print('Already count: ', all['count'])
        f.close()
        for d in all['data'].values():
            visited.add(d['url'])
    except Exception as e:
        all = {
            'name': '新浪体育',
            'count': 0,
            'data': {}
        }
    
    initLink()
    f = open('nba.json', 'w')
    for link in links:
        try:
            get_page(link,all)
        except Exception as e:
            print('*** search {} Error!!! {}'.format(link, e))

    json.dump(all, f)
    f.close()

if __name__ == '__main__':
    main()