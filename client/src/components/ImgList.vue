<template>
  <transition name="expand">
    <!-- <v-list class="expand-list">
      <v-list-item v-for="(file, i) in files" :key="i" class="pl-0">
        <thumb-item :file="file"></thumb-item>
      </v-list-item>
    </v-list> -->
    <v-treeview class="expand-list" item-children="items" :items="files" dense>
        <template v-slot:prepend="{item}">
            <thumb-item v-if="item.type==='file'" :file="item.id"></thumb-item>
        </template>
    </v-treeview>
  </transition>
</template>

<script>
import { mapGetters } from "vuex";
// import * as a from "../store/action_types";
// import * as m from "../store/mutation_types";
import ThumbItem from "./ThumbItem";

export default {
    name: "ImgList",
    components: {
        ThumbItem
    },
    data: function () {
        return {
        };
    },
    computed: {
        ...mapGetters(["files"])
    },
    mounted: function () {
        // const projectId = this.$route.params.projectId;
        // const studyId = this.$route.params.studyId;
        // const serieId = this.$route.params.serieId;

        // this.$store.commit(m.PROJECTS_SET_ACTIVE, {
        //     projectId: projectId,
        //     studyId: studyId,
        //     serieId: serieId,
        // });

        // this.$store.dispatch(a.PROJECTS_FETCH_FILES);
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