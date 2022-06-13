from postdb.models import *
from django.db import transaction
from django.core.management.base import BaseCommand
from bs4 import BeautifulSoup
from datetime import datetime
import jieba,json,lxml
import jieba.analyse

class Command(BaseCommand):
    name = 'nba'
    path = '../spider/'

    idx = dict()

    def segment(self, NID, time, text, title):
        b = dict()
        def seg(txt, weight):
            #txt:str, weight:int
            #data = jieba.lcut_for_search(txt)
            data = jieba.analyse.extract_tags(txt, topK=100, withWeight=True)
            for x in data:
                a = b.get(x[0])
                if not a:
                    b[x[0]] = weight*x[1]
                else:
                    b[x[0]]+= weight*x[1]
        seg(title, 20)
        seg(text, 1)

        for (key, item) in b.items():
            a = self.idx.get(key)
            t = [item, NID]
            if not a:
                self.idx[key] = [t]
            else:
                self.idx[key].append(t)

    @transaction.atomic
    def work(self):
        # 加载数据前务必清空数据库
        print("====== start loading  ======")
        filename = self.path + self.name + '.json'
        all = dict()
        try:
            f = open(filename, 'r')
            all = json.loads(f.read())
            print('Count: ', all['count'])
            f.close()
        except Exception as e:
            print(e)
            return

        for (TID, tmp) in all['data'].items():
            ID = int(TID.split('_')[-1])
            print('excuting {} / {}'.format(ID, all['count']))
            try:
                NID = ID
                time = datetime.strptime(tmp.get('stamp', ''), '%Y年%m月%d日 %H:%M')
                title = tmp.get('title', '')
                content = tmp.get('content', '')

                bs = BeautifulSoup(content, 'lxml')
                plain = bs.get_text()
                self.segment(NID, datetime.strftime(time, '%Y-%m-%d-%H-%M-%S'), plain, title)

                data = PostInfo()
                data.NID = NID
                data.TID = TID
                data.time = time
                data.title = title
                data.content = content
                data.plain = plain
                data.url = tmp.get('url', '')
                source = tmp.get('source')
                if source:
                    data.sourceLink = source.get('link', '')
                    data.sourceText = source.get('text', '')
                data.save()
            except Exception as e:
                print(e)

        print('====== building words list ======')
        cnt = 0
        for (_key, _item) in self.idx.items():
            cnt += 1
            print('word: {} count: {} / {}'.format(_key,cnt, len(self.idx)))

            arr = []
            arr.extend(_item)
            IndexInfo.objects.create(key=_key, value=json.dumps(arr))

    def handle(self, *args, **options):
        self.work()