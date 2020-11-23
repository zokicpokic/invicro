<template>
  <transition name="expand">
    <v-treeview class="expand-list" item-children="items" :items="files" dense>
      <template v-slot:prepend="{item}">
        <thumb-item v-if="item.type==='file'" :file="item.id"></thumb-item>
      </template>
    </v-treeview>
  </transition>
</template>

<script>
import {mapGetters} from "vuex";
import ThumbItem from "./ThumbItem";

export default {
    name: "ImgList",
    components: {
        ThumbItem
    },
    data: function () {
        return { };
    },
    computed: {
        ...mapGetters(["files"])
    }
};
</script>

<style scoped>
.expand-list {
  width: 200px;
}
.expand-enter-active, .expand-leave-active {
  transition: all .7s ease;
}
.expand-enter, .expand-leave-to {
  width: 0;
  opacity: 0;
}
</style>