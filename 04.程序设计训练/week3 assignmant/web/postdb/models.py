from django.db import models

class PostInfo(models.Model):
    NID = models.IntegerField()
    TID = models.CharField(max_length=20)
    time = models.DateTimeField()
    title = models.TextField()
    content = models.TextField()
    plain = models.TextField()
    url = models.TextField()
    sourceLink = models.TextField()
    sourceText = models.TextField()

    def __str__(self):
        return 'NID: {}, TID: {}, title: {}, time: {}, url: {}'.format(self.NID, self.TID, self.title, self.time, self.url)

class IndexInfo(models.Model):
    key = models.TextField()
    value = models.TextField() 
    # {[weight, NID]}

    def __str__(self):
        return 'key: {}'.format(self.key)

class TeamInfo(models.Model):
    name = models.CharField(max_length=20)
    content = models.TextField()

    def __str__(self):
        return 'team name:{}'.format(name)

class PlayerInfo(models.Model):
    name = models.CharField(max_length=20)
    team = models.CharField(max_length=20)

    def __str__(self):
        return 'player name:{}, team:{}'.format(name,team)