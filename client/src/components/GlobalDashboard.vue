<!-- Copyright (c) 2020-. Authors: see NOTICE file.-->


<template>
  <v-container fluid pa-0>
    <v-row no-gutters>
      <v-col cols="12">

    <v-toolbar dense flat :dark="true">
      <v-toolbar-title></v-toolbar-title>
      <v-btn v-on:click="showNewClass" icon>
        <v-icon>mdi-briefcase-plus-outline</v-icon>
      </v-btn>
      <!--<v-btn v-on:click="addFearure" icon>
        <v-icon>mdi-puzzle-edit-outline</v-icon>
      </v-btn>-->
      <v-btn disabled icon>
        <v-icon>mdi-reply</v-icon>
      </v-btn>
      <v-btn disabled icon>
        <v-icon>mdi-share</v-icon>
      </v-btn>
      <v-btn v-on:click="showClasses" icon>
        <v-icon>mdi-set-right</v-icon>
      </v-btn>
      <p>{{ activeClassName }}</p>
      <!--<v-switch label="Display MLD File" color="primary" value="red" hide-details></v-switch>-->
      <v-spacer></v-spacer>
    </v-toolbar>

    <v-toolbar dense flat :dark="false">
      <v-toolbar-title></v-toolbar-title>
      <v-btn-toggle v-model="selectedGeometry" mandatory>
        <v-btn v-for="item in geometries" :key="item.name">
          <v-icon>{{ item.icon }}</v-icon>
        </v-btn>
      </v-btn-toggle>
      <v-spacer></v-spacer>
    </v-toolbar>

        <v-row no-gutters>
            <v-col cols="2">
                <img-list></img-list>
            </v-col>
            <v-col cols="10">


                    <AnnotationSettings :visible="showAnnotationSettings" @close="showAnnotationSettings=false" />
                    <ClassSettings :visible="showClassSettings" @close="showClassSettings=false" />
                    <NewClassSettings :visible="showNewClassSettings" @close="showNewClassSettings=false" />
                    <pathology-image-viewer />
            </v-col>
        </v-row>
      </v-col>
    </v-row>
  </v-container>
</template>

<script>
import PathologyImageViewer from "@/components/viewer/PathologyImageViewer";
import AnnotationSettings from "@/components/AnnotationsSettings";
import ClassSettings from "@/components/ClassSettings";
import NewClassSettings from "@/components/NewClassSettings";
import ImgList from "./ImgList";
import { mapGetters } from "vuex";
import * as m from "../store/mutation_types";
import * as a from "../store/action_types";
import axios from "axios";
import constants from "@/utils/constants";

export default {
  name: "global-dashboard",
  data() {
    return {
      showAnnotationSettings: false,
      showClassSettings: false,
      showNewClassSettings: false,
      geometries: [
        {
          name: "Polygon",
          icon: "mdi-vector-polygon",
        },
        {
          name: "Ellipse",
          icon: "mdi-vector-ellipse",
        },
        /*{
          name: "Circle",
          icon: "mdi-vector-circle",
        },*/
        {
          name: "Rectangle",
          icon: "mdi-vector-rectangle",
        }/*,
        {
          name: "Square",
          icon: "mdi-vector-square",
        }*/,
      ],
    };
  },
  computed: {
    ...mapGetters(["annotation", "activeGeometry", "activeClassName"]),
    selectedGeometry: {
      get: function () {
        return this.geometries.map((x) => x.name).indexOf(this.activeGeometry);
      },
      set: async function (val) {
        const geometry = this.geometries[val].name;
        this.$store.commit(m.PROJECTS_SET_ACTIVE_GEOMETRY, {
          geometry,
        });
        var f = this.$store.dispatch(a.PROJECTS_POST_ANNOTATION, {
          annotation: this.annotation,
        });

        await Promise.all([f])
          .then(() => {})
          .catch((e) => {
            console.log("error fetcing activeGeometry data");
            console.log(e);
          });
      },
    },
  },
  components: {
    PathologyImageViewer,
    AnnotationSettings,
    ClassSettings,
    NewClassSettings,
    ImgList
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
    let Settings;
    await axios.get('configuration.json').then(response => (Settings = response.data));
    console.log(Settings);

    for (var i in constants) {
      // eslint-disable-next-line no-prototype-builtins
      if (Settings.hasOwnProperty(i)) {
        //constants[i] = Settings[i];
        console.log(Settings[i]);
        console.log(constants[i]);
      }
    }
    // Object.freeze(constants);
    // await Promise.all(
    //   [
    //     //TO DO WAIT FOR DEFAULT FETCH MODEL
    //   ].map((p) => p.catch((e) => console.log(e)))
    // );
  },
};
</script>

<style scoped>
/* td {
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
} */
</style>
