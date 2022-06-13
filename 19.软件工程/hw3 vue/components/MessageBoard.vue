<template>
    <div id="message-board">
        <el-container style="height:100%; border: 1px solid #eee">
            <el-header style="text-align: right; font-size: 10px">
                <el-button style="display: inline-block;margin-right: 15px;" v-on:click="postDialog.dialogVisible=true">
                    <i class="el-icon-edit">发表</i>
                </el-button>
                <!--请修改这两行注释中间的代码完成"刷新"功能-->
                <el-button style="display: inline-block;margin-right: 15px;"
                @click = "getMsg">
                <!--请修改这两行注释中间的代码完成"刷新"功能-->
                    <i class="el-icon-refresh" style="object-fit: fill">刷新</i>
                </el-button>
            </el-header>

            <el-main>
              <!--请补全这两行注释中间的messagelist-->
              <MessageList v-bind:messageList="messageList"></MessageList>
              <!--请补全这两行注释中间的messagelist-->
            </el-main>

            <el-footer>@CST2020SE</el-footer>
        </el-container>
        <!--请补全这两行注释中间的PostDialog-->
        <PostDialog 
        :state = "state"
        :dialogVisible= "postDialog.dialogVisible"
        @createMsg = "createMsg"
        @closeDialog="closeDialog"></PostDialog>
        <!--请补全这两行注释中间的PostDialog-->
        <el-dialog
                style="text-align: center"
                :title="alertDialog.text"
                :visible.sync="alertDialog.dialogVisible"
                width="40%">
        </el-dialog>
    </div>
</template>

<script>
    import MessageList from "@/components/MessageList"
    import PostDialog from "@/components/PostDialog"
    import { httpMsgGet, httpMsgPost } from "@/utils/communication"
    export default {
        name: "MessageBoard",
        components: {
            MessageList,
            PostDialog
        },
        // 请在下方设计自己的数据结构及函数来完成最终的留言板功能
        data(){
            var name = document.cookie.match(new RegExp("user=([^;]*)"))
            var valid = false
            if (name !== null){
                name = name[1]
                valid = true
            }
            else
                name = ""
            return {
                postDialog:{
                  dialogVisible:false,
                  form:{
                      title:"",
                      content:""
                    }
                },
                alertDialog:{
                    "text":"",
                    dialogVisible:false
                },
                state:{
                    username: name,
                    username_valid: valid
                },
                messageList: []
            }
        },
        methods:{
            closeDialog: function () {
                this.postDialog.dialogVisible = false
            },
            createMsg: function (title, content, username) {
                const res = httpMsgPost(title, content, username)
                this.closeDialog()
                if (res.code==201 || res=="OK")  {
                    this.alertDialog.text = '发表成功';
                }
                else {
                    this.alertDialog.text = '发表失败:' + res;
                }
                this.alertDialog.dialogVisible = true
                this.getMsg()
            },
            getMsg: function () {
                this.messageList = httpMsgGet()
            }
        },
    }
</script>

<style scoped>
    #message-board{
        height: calc(100vh - 16px);
    }
    .message-tab{
        display: block;
        padding: 10px;
        text-align: left;
    }
    .el-header {
        background-color: #B3C0D1;
        color: #333;
        line-height: 60px;
    }
    .el-footer {
        background-color: #B3C0D1;
        color: #333;
        line-height: 60px;
    }
    .el-aside {
        color: #333;
    }
</style>
