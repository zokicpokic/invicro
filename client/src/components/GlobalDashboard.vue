<!-- Copyright (c) 2020-. Authors: see NOTICE file.-->


<template>
  <div>
    <!--<v-card color="grey" tile>-->
    <v-toolbar dense :dark="true">
      <v-toolbar-title></v-toolbar-title>
      <v-btn v-on:click="showNewClass" icon>
        <v-icon>mdi-briefcase-plus-outline</v-icon>
      </v-btn>
      <v-btn v-on:click="addFearure" icon>
        <v-icon>mdi-puzzle-edit-outline</v-icon>
      </v-btn>
      <v-btn icon>
        <v-icon>mdi-reply</v-icon>
      </v-btn>
      <v-btn icon>
        <v-icon>mdi-share</v-icon>
      </v-btn>
      <v-btn v-on:click="showClasses" icon>
        <v-icon>mdi-set-right</v-icon>
      </v-btn>
      <!--<v-switch label="Display MLD File" color="primary" value="red" hide-details></v-switch>-->
      <v-spacer></v-spacer>
    </v-toolbar>
    <!--</v-card>-->
    <v-main>
      <AnnotationSettings :visible="showAnnotationSettings" @close="showAnnotationSettings=false" />
      <ClassSettings :visible="showClassSettings" @close="showClassSettings=false" />
      <NewClassSettings :visible="showNewClassSettings" @close="showNewClassSettings=false" />
      <pathology-image-viewer />
    </v-main>
  </div>
</template>

<script>
import PathologyImageViewer from "@/components/viewer/PathologyImageViewer";
import AnnotationSettings from "@/components/AnnotationsSettings";
import ClassSettings from "@/components/ClassSettings";
import NewClassSettings from "@/components/NewClassSettings";

export default {
  name: "global-dashboard",
  data() {
    return {
      showAnnotationSettings: false,
      showClassSettings: false,
      showNewClassSettings: false,
    };
  },
  components: {
    PathologyImageViewer,
    AnnotationSettings,
    ClassSettings,
    NewClassSettings,
  },
  methods: {
    addFearure: function () {
      this.showAnnotationSettings = true;
    },
    showClasses: function () {
      this.showClassSettings = true;
    },
    showNewClass: function () {
      this.showNewClassSettings = true;
    },
  },
  async created() {
    await Promise.all(
      [
        //TO DO WAIT FOR DEFAULT FETCH MODEL
      ].map((p) => p.catch((e) => console.log(e)))
    );
  },
};
</script>

<style scoped>
td {
  vertical-align: middle !important;
}

a.project-name {
  font-weight: 600;
  font-size: 1.1rem;
}

.all-projects {
  margin-top: 1rem;
}

.right-column {
  display: flex;
  flex-direction: column;
}

.stats {
  padding-bottom: 2.5rem;
}

td:first-child {
  font-weight: 600;
  text-align: right;
  width: 25%;
}

.fas.fa-info-circle {
  cursor: pointer;
}
</style>
