from django.shortcuts import render
from django.http import JsonResponse
from postdb.models import *
from datetime import datetime
import json,time,jieba,re
import jieba.analyse

def index(request):
    return render(request, "index.html")

def ajax_total(request):
    return JsonResponse({'total': PostInfo.objects.count()})

def search(request):
    kw = request.GET.get('kw', '') #keyword
    if not kw:
        return render(request, "index.html")
    return render(request, "search.html", {'kw': kw})


def ajax_search(request):
    kw = request.GET.get('kw', '')
    if not kw:
        return JsonResponse({})
    start_time = time.time()
    kw = kw.replace(' ','')  # necessary
    words = jieba.lcut(kw)
    news = dict()
    keywords = jieba.analyse.extract_tags(kw,withWeight=True) #tf-idf分词，分成二维列表
    for i in keywords:
        tmp = []
        try:
            tmp = json.loads(IndexInfo.objects.get(key=i[0]).value)
            for j in tmp:
                if news.get(j[1]):#以NID作内部索引
                    news[j[1]]+=j[0]*i[1]
                else:
                    news[j[1]]=j[0]*i[1]
        except Exception as e:
            print(e)
            continue
    news = list(news.items())
    news.sort(key = lambda x:x[1],reverse=True)
    news = [news[k][0] for k in range(len(news))]
    return JsonResponse({
        'len':len(news),
        'cost':'{:.3f}'.format(time.time()-start_time),
        'words':words,
        'news':news
        })

def ajax_post_brief(request):
    if not len(request.GET):
        return JsonResponse({})
    news_brief = dict()
    
    for i in range(len(request.GET)):
        try:
            id = request.GET.get(str(i))
            tmp = PostInfo.objects.get(NID=id)
            plain = tmp
            news_brief[i]={
                'TID': tmp.TID,
                'title': tmp.title,
                'brief': str(tmp.plain)[:100],
                'time': tmp.time.strftime('%Y/%m/%d %H:%M'),
                'source': tmp.sourceText,
            }
        except:
           continue
    return JsonResponse(news_brief)

def post(request):
    id = request.GET.get('id','')
    tmp = None
    try:
        tmp = PostInfo.objects.get(TID=id)
    except Exception as e:
        print(e)
        return render(request, 'error.html')
    return render(request, 'post.html', {
        'NID': tmp.NID,
        'title': tmp.title,
        'time': datetime.strftime(tmp.time, '%Y-%m-%d %H:%M'),
        'content': tmp.content,
        'source_link': tmp.sourceLink,
        'source_text': tmp.sourceText,
        'origin_url': tmp.url
    })

def team(request):
    team_name = request.GET.get('name','')
    tmp = None
    try:
        tmp = TeamInfo.objects.get(name=team_name)
    except Exception as e:
        print(e)
        return render(request, 'error.html')
    
    return render(request, 'team.html',{
        'name' : team_name,
        'content' : tmp.content
        })

def ajax_post_links(request):
    #更新新闻页面中的链接
    teams = TeamInfo.objects.all()
    players = PlayerInfo.objects.all()
    team_names = [ x.name for x in teams ]
    player_index = dict()
    for player in players:
        player_index[player.name]= player.team
    return JsonResponse({
        'teams':team_names,
        'players':player_index
        }) 
    
def team_rank(request):
    tmp =TeamInfo.objects.all()
    teams = [ x.name for x in tmp ]
    ans = []
    
    for team in teams:
        kw = jieba.analyse.extract_tags(team)
        cnt = 0
        for i in kw:
            tmp = json.loads(IndexInfo.objects.get(key=i).value)
            cnt += len(tmp)
        ans.append({'cnt':cnt,'name':team})
    ans.sort(key = lambda x:x['cnt'],reverse=True)

    return render(request,'team_rank.html',{'ans':ans})