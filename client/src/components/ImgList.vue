<template>
  <v-list>
    <v-list-item v-for="(file, i) in files" :key="i">
      <thumb-item :file="file"></thumb-item>
    </v-list-item>
  </v-list>
</template>

<script>
import {mapGetters} from "vuex";
import * as a from "../store/action_types";
import * as m from "../store/mutation_types";
import ThumbItem from "./ThumbItem";

export default {
  name: "ImgList",
  components: {
    ThumbItem
  },
  data: function () {
    return {};
  },
  computed: {
    ...mapGetters(["files"])
  },
  watch: {
    $route: function () {
      console.log("Route changed");
      const projectId = this.$route.params.projectId;
      const studyId = this.$route.params.studyId;
      const serieId = this.$route.params.serieId;

      this.$store.commit(m.PROJECTS_SET_ACTIVE, {
        projectId: projectId,
        studyId: studyId,
        serieId: serieId,
      });

      this.$store.dispatch(a.PROJECTS_FETCH_FILES);
    }
  },
  mounted: function () {
    const projectId = this.$route.params.projectId;
    const studyId = this.$route.params.studyId;
    const serieId = this.$route.params.serieId;

    this.$store.commit(m.PROJECTS_SET_ACTIVE, {
      projectId: projectId,
      studyId: studyId,
      serieId: serieId,
    });

    this.$store.dispatch(a.PROJECTS_FETCH_FILES);
  }
};
</script>