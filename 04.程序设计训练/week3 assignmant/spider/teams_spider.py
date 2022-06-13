from requests_html import HTMLSession
from urllib.parse import urlparse
import json,re

session = HTMLSession();
ua = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/76.0.3809.132 Safari/537.36"
root ="https://www.qiumiwu.com/team"
links = set()

def get_pages(link,all):
    def escape(x):
        x = x.replace(u'\xa0', u' ')
        x = x.replace(u'\u3000', u' ')
        x = x.strip()
        x = re.sub(r'<img[\s\S]*?>','',x)
        x = re.sub(r'\/player\/\w+','#',x)
        return x
    r = session.get(link,headers={'user_agent':ua})
    name = r.html.find('body > div.main1200.float_center.overflow_h > div.top-dark.bg-f1.mb_15 > a.color-a5',first=True).text
    content = r.html.find('body > div.main1200.float_center.overflow_h > div.detail100 > div.detail.overflow_h.pt_20',first=True).html
    content = escape(content)
    players = r.html.find('#slideBox > div > ul',first=True).text
    tmp_list = players.split('\n')
    players_list = []
    for i in tmp_list:
        i.replace('.','-')
        i.replace(' ','-')
        players_list.append(i.split('-')[-1])
    all[name]={
        'players' : players_list,
        'content' : content
        }
    
def main():
    r = session.get(root,headers={'user_agent':ua})
    for link in r.html.links:
        if(link.startswith(root+'/')):
            links.add(link)

    all = dict()
    for link in links:
        get_pages(link,all)

    f = open('teams.json', 'w')
    json.dump(all, f)
    print('end')
    f.close()

if __name__ == '__main__':
    main()