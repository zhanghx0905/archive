from requests_html import HTMLSession
import jieba
from wordcloud import WordCloud
import time
def get_news():
    print('开始爬取热点新闻')
    ans_news_titles = []
    session = HTMLSession()
    # 获取百度新闻
    r = session.get('https://news.baidu.com/')
    title1_baidu = r.html.find('#pane-news > div > ul > li.hdline0 > strong > a', first=True)
    ans_news_titles.append(title1_baidu.text)
    titles_baidu = r.html.find('#pane-news > ul:nth-child(n) > li.bold-item > a')
    for title in titles_baidu:
        ans_news_titles.append(title.text)
    
    # 获取网易新闻
    r = session.get('https://news.163.com/')
    title1_163 = r.html.find('#js_top_news > h2:nth-child(1) > a', first=True)
    title2_163 = r.html.find('#js_top_news > h2.top_news_title > a', first=True)
    titles_163 = r.html.find('#js_top_news > ul:nth-child(n) > li:nth-child(n)')
    ans_news_titles.append(title1_163.text)
    ans_news_titles.append(title2_163.text)
    for title in titles_163:
        ans_news_titles.append(title.text)
    word_jieba = jieba.cut(' '.join(ans_news_titles), cut_all=True)
    word_split = " ".join(word_jieba)
    my_wordclud = WordCloud(background_color='white', font_path = 'C:\Windows\Fonts\simhei.ttf', max_words = 150, width = 1600, height = 800)
    # 生成词云
    my_wordclud = my_wordclud.generate(word_split)
    # 以当前时间为名称存储词云
    now = time.strftime('%Y-%m-%d-%H_%M_%S', time.localtime(time.time())) 
    my_wordclud.to_file(now + '.png')


get_news()