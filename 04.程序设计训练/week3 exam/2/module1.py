from html.parser import HTMLParser
import csv
import re
lis = []


class myParser(HTMLParser):
    def __init__(self):
        HTMLParser.__init__(self)
        self.data = []
        self.row = []
        self.table_cnt = 0
        self.readrow = False
        self.work = False

    def handle_starttag(self, tag, attrs):
        if tag == 'tbody':
            self.data = []
        elif tag == 'tr':
            self.readrow = True
            self.row = []
        elif tag == 'td' or tag == 'th':
            self.work = True

    def handle_endtag(self, tag):
        if tag == 'tr':
            self.readrow = False
            while '' in self.row:
                self.row.remove('')
            while u'\xa0' in self.row:
                self.row.remove(u'\xa0')
            self.data.append(self.row)
        elif tag == 'tbody':

            file = open(str(lis[self.table_cnt])+'.csv',
                        'w', newline='', errors='ignore')
            csvwriter = csv.writer(file)
            csvwriter.writerows(self.data)
            file.close()
            self.table_cnt += 1
            print('table end')
        elif tag == 'td' or tag == 'th':
            self.work = False

    def handle_data(self, data):
        data = data.replace('\n', '')
        data = data.replace('\t', '')
        data = data.replace(' ', '')
        if data == '':
            return
        if self.readrow and self.work:
            self.row.append(data)
            print(data)


f = open('origin.html', 'r', errors='ignore', encoding='utf-8')
raw = f.read()


def escape(x):
    x = x.replace(u'\xa0', u' ')
    x = x.replace(u'\u3000', u' ')
    x = x.strip()
    return x


raw = escape(raw)
lis = re.findall(r'<h3>([A-Z])</h3>', raw)
parser = myParser()
parser.feed(raw)

ans = re.sub(r'<\/tbody>[\w\W]*?<tbody>', '', raw)
ans = re.sub(r'<tr bgcolor="#99CCFF">[\w\W]*?<\/tr>', '', ans)
with open('all.html', 'w', errors='ignore', encoding='utf-8') as f:
    f.write(ans)
