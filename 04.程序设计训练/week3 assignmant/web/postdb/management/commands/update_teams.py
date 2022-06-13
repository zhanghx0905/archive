from postdb.models import *
from django.db import transaction
from django.core.management.base import BaseCommand
import json

class Command(BaseCommand):
    name = 'teams'
    path = '../spider/'

    @transaction.atomic
    def work(self):
        print("====== start loading {} ======".format(self.name))
        filename = self.path + self.name + '.json'
        all = dict()
        try:
            f = open(filename, 'r')
            all = json.loads(f.read())
            f.close()
        except Exception as e:
            print(e)
            return
        for name in all.keys():
            data = TeamInfo()
            data.name = name
            data.content = all[name]['content']
            players_list = all[name]['players']
            for player_name in players_list:
                tmp = PlayerInfo()
                tmp.name = player_name
                tmp.team = name
                tmp.save()
            data.save()
            print('{} finished'.format(name))

    def handle(self, *args, **options):
        self.work()