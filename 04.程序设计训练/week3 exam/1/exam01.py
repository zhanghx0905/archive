# python3
import sys,os

if(len(sys.argv)<2):
    print("Error1")
    sys.exit(0)


op = sys.argv[1]
key = []

weight = {'3':1,'4':2,'5':3,'6':4,'7':5,'8':6,'9':7,'10':8,'J':9,
          'Q':10,'K':11,'A':12,'2':13}

t = {'S':1,'H':2,'C':3,'D':4}


def check(key):
    key = [x[1:] for x in key]
    if len(key)==1:
        return '1'
    else:
        SHUN =True
        for i in range(1,len(key)):
            if weight[key[i]]-weight[key[i-1]]!=1:
                SHUN=False
        if SHUN:
            return '2'
        else:
            ZHADAN = True
            SAN = False
            tmp = weight[key[0]]
            for i in range(1,len(key)):
                if weight[key[i]]!=tmp:
                    ZHADAN = False
                    if not SAN:
                        SAN =True
                    else:
                        SAN =False
                        break
            if(ZHADAN):
                return '4'
            elif (SAN):
                return '3'
    return '0'
        
def cmp(a,b):
    if weight[a[1:]]!=weight[b[1:]]:
        if weight[a[1:]]>weight[b[1:]]:
            return 1
        else:
            return -1
    if t[a[0]]>t[b[0]]:
        return 1
    elif t[a[0]]<t[b[0]]:
        return -1
    return 0
try:
    if op=='-sort':
        if(len(sys.argv)!=3):
            print("Error2")
            sys.exit(0)
   
        key = sys.argv[2].split(',')
        for i in range(len(key)):
            for j in range(i+1,len(key)):
                mark =False
                if  weight[key[i][1:]] != weight[key[j][1:]]:
                    if weight[key[i][1:]] > weight[key[j][1:]]:
                        key[i],key[j]=key[j],key[i]
                else:
                    if t[key[i][0]] > t[key[j][0]]:
                        key[i],key[j]=key[j],key[i]
        for i in range(len(key)):
            if(i!=0):
                 print(','+key[i],end='')
            else:
                print(key[i],end='')
    

    elif op=='-check':
        if(len(sys.argv)!=3):
            print("Error2")
            sys.exit(0)
        key = sys.argv[2].split(',')
        print(check(key))
       

    elif op=='-cmp':
        if(len(sys.argv)!=4):
            print("Error2")
            sys.exit(0)
        key1 =sys.argv[2].split(',')
        key2 = sys.argv[3].split(',')
        if (key1[-1]==key2[-1]):
            print("Error4")
            sys.exit(0)
        a,b= check(key1),check(key2)
        #print(a,b)
        if a!=b and a!='4' and b!='4':
            print('=')
        elif a!=b and a=='4':
            print('>')
        elif a!=b and b=='4':
            print('<')
        else: #shun error
            ans = cmp(key1[0],key2[0])
            if ans==1:
                print('>')
            elif ans==-1:
                print('<')
            else: 
                print('=')

    else:
        print("Error1")
        sys.exit(0)

except SystemExit:
    pass
except:
    print('Error3')