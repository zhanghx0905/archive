from django.shortcuts import render
import re
def index(request):
    return  render(request,'index.html')
def get_data(request):
    name = request.GET.get('name',None)
    email = request.GET.get('email',None)
    sex = request.GET.get('sex',None)
    date = request.GET.get('date',None)
    if not re.match(r'^[a-zA-Z0-9_-]+@[a-zA-Z0-9_-]+(\.[a-zA-Z0-9_-]+)+$',email):
        info='电子邮件格式不正确'
        return render(request,'index.html',{'info':info,'sex':sex,'name':name,'email':email,'date':date})
    if not (name and email and sex and date):
        info='表单项不能为空'
        return render(request,'index.html',{'info':info,'sex':sex,'name':name,'email':email,'date':date})

    with open('data.txt','a+') as f:
        f.write('{}--{}--{}--{}\n'.format(name,sex,date,email))
    info = '录入成功，请继续录入下一条信息'
    return render(request,'index.html',{'info':info})

def login(request):
    data=[]
    with open('data.txt','r+') as f:
        for line in f:
            item = line.replace('\n','').split('--')
            data.append({'name':item[0],'sex':item[1],'date':item[2],'email':item[3]})

    return render(request,'login.html',{'data':data}) 