<template>
  <div id="stage3">
    <!--修改下方的MessageList和PostDialog-->
    <MessageList v-bind:messageList="messageList"></MessageList>
    <PostDialog 
    :dialogVisible="dialogVisible"
    :state = "state"
    @createMsg="createMsg"
    @closeDialog="closeDialog"
    ></PostDialog>
  </div>
</template>

<script>
import MessageList from "@/components/MessageList.vue"
import PostDialog from "@/components/PostDialog.vue"

export default {
  name: 'Stage3',
  components: {
    MessageList,
    PostDialog
  },
  // 请在下方设计自己的数据结构和响应函数
  data(){
    return {
      messageList: [],
      dialogVisible: true,
      state:{
        username:  "",
        username_valid:false
      }
    }
  },
  methods: {
      closeDialog: function() {
            this.dialogVisible = false
      },
      createMsg: function (_title, _content) {
        this.messageList.push({
            title: _title,
            content: _content,
            user: this.state.username,
            timestamp: new Date().getTime()
        })
        this.closeDialog()
      }

  }
}
</script>