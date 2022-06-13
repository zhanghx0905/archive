import json
from json import JSONDecodeError

from django.core.exceptions import ValidationError
from django.http import HttpResponse, JsonResponse
from django.shortcuts import render

from .models import Message, User


# Create your views here.
def message(request):
    def gen_response(code: int, data: str):
        return JsonResponse({
            'code': code,
            'data': data
        }, status=code)

    # GET的完整实现已经给出，同学们无需修改
    if request.method == 'GET':
        limit = request.GET.get('limit', default='100')
        offset = request.GET.get('offset', default='0')
        if not limit.isdigit():
            return gen_response(400, '{} is not a number'.format(limit))
        if not offset.isdigit():
            return gen_response(400, '{} is not a number'.format(offset))

        return gen_response(200, [
            {
                'title': msg.title,
                'content': msg.content,
                'user': msg.user.name,
                'timestamp': int(msg.pub_date.timestamp())
            }
            for msg in Message.objects.all().order_by('-pk')[int(offset): int(offset) + int(limit)]
        ])

    elif request.method == 'POST':
        # 从cookie中获得user的名字，如果user不存在则新建一个
        # 如果cookie中没有user则使用"Unknown"作为默认用户名
        name = request.COOKIES['user'] if 'user' in request.COOKIES else 'Unknown'
        user = User.objects.filter(name=name).first()
        if not user:
            user = User(name=name)
            try:
                # 注意在调用full_clean()时Django会自动检测字段的有效性，这个有效性检测包括检测CharField是否满足最大长度限制
                user.full_clean()
                # 存入数据库
                user.save()
            except ValidationError as e:
                return gen_response(400, "Validation Error of user: {}".format(e))

        try:
            input_data = json.loads(request.body)
        except JSONDecodeError as e:
            return gen_response(400, f"Validation Error of input format: {e}. input = {request.body}")

        title = input_data.get('title', None)
        content = input_data.get('content', None)
        if title is None or content is None:
            return gen_response(400, "Title or content are not given")
        msg = Message(user=user, title=title, content=content)
        try:
            msg.full_clean()
            msg.save()
        except ValidationError as e:
            return gen_response(400, "Validation Error of message: {}".format(e))

        return gen_response(201, "message was sent successfully")

    else:
        return gen_response(405, 'method {} not allowd'.format(request.method))
