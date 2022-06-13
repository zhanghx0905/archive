/**
 * 如果需要修改为正常上线模式，请注释下面mock的import代码
 * **/
//import "@/mock/index"

 // 请在下方实现自己的后端通信函数
import API from "@/utils/API"
export function httpMsgGet() {
    const req = new XMLHttpRequest()
    req.open(API.GET_MESSAGE_LIST.method, API.GET_MESSAGE_LIST.path, false)
    req.send()
    const raw = JSON.parse(req.responseText).data
    var msgList = []
    for (const idx in raw) {
        msgList.push({
            title: raw[idx].title,
            content: raw[idx].content,
            user: raw[idx].user,
            timestamp: raw[idx].timestamp*1000
        })
    }
    return msgList.reverse()
}

export function httpMsgPost(title, content, user) {
    const req = new XMLHttpRequest()
    req.open(API.POST_NEW_MESSAGE.method, API.POST_NEW_MESSAGE.path, false)
    const cookie = "user=" + escape(user) + "; expires=Thu, 18 Dec 2043 12:00:00 GMT"
    document.cookie = cookie
    const obj = {
        "title": title,
        "content": content
    }
    req.send(JSON.stringify(obj))
    
    return JSON.parse(req.responseText)
}
