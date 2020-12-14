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
          <v-btn :disabled="!canUndo" v-on:click="performUndo" icon>
            <v-icon>mdi-reply</v-icon>
          </v-btn>
          <v-btn :disabled="!canRedo" v-on:click="performRedo" icon>
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
          <v-divider class="mx-4" vertical></v-divider>
          <contrast-brightness-settings
            v-model="contrastVal"
            :title="'CONTRAST SETTINGS'"
            :icon="'mdi-contrast-circle'"
            @reset="resetContrast()"/>
          <contrast-brightness-settings
            v-model="brightnessVal"
            :title="'BRIGHTNESS SETTINGS'"
            :icon="'mdi-brightness-6'"
            @reset="resetBrightnrss()"/>
          <color-settings />
        </v-toolbar>

        <v-row no-gutters>
          <v-card class="ma-0 pa-0" flat tile >
            <v-navigation-drawer width="100%" permanent>
              <v-row class="fill-height d-flex flex-row overflow-x-hidden" no-gutters>
                <v-navigation-drawer mini-variant mini-variant-width="50" permanent>
                  <v-list-item class="d-flex justify-center px-2">
                    <v-icon v-if="showDrawer" @click="showDrawer = false">mdi-chevron-left</v-icon>
                    <v-icon v-else @click="showDrawer = true">mdi-chevron-right</v-icon>
                  </v-list-item>
                </v-navigation-drawer>
                <img-list v-show="showDrawer"></img-list>
              </v-row>
            </v-navigation-drawer>
          </v-card>

          <v-col>
            <annotation-settings :visible="showAnnotationSettings" @close="showAnnotationSettings=false" />
            <class-settings :visible="showClassSettings" @close="showClassSettings=false" />
            <new-class-settings :visible="showNewClassSettings" @close="showNewClassSettings=false" />
            <pathology-image-viewer />
          </v-col>
        </v-row>

      </v-col>
    </v-row>
  </v-container>
</template>

<script>
import ContrastBrightnessSettings from "./ContrastBrightnessSettings";
import ColorSettings from "./ColorSettings";
import PathologyImageViewer from "@/components/viewer/PathologyImageViewer";
import AnnotationSettings from "@/components/AnnotationsSettings";
import ClassSettings from "@/components/ClassSettings";
import NewClassSettings from "@/components/NewClassSettings";
import ImgList from "./ImgList";
import {mapGetters} from "vuex";
import * as m from "../store/mutation_types";
import * as a from "../store/action_types";
import axios from "axios";
import constants from "@/utils/constants";

export default {
    name: "GlobalDashboard",
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
                }*/
            ],
            showDrawer: true,
            timer: null
        };
    },
    computed: {
        ...mapGetters([
            "annotation",
            "activeGeometry",
            "activeClassName",
            "canUndo",
            "canRedo",
            "brightness",
            "contrast"
        ]),
        selectedGeometry: {
            get: function () {
                return this.geometries.map((x) => x.name).indexOf(this.activeGeometry);
            },
            set: function (val) {
                const geometry = this.geometries[val].name;
                this.$store.commit(m.PROJECTS_SET_ACTIVE_GEOMETRY, {
                    geometry,
                });
                this.$store.dispatch(a.PROJECTS_POST_ANNOTATION, {
                    annotation: this.annotation,
                }).then(() => {
                }).catch((e) => {
                    console.log("error fetcing activeGeometry data");
                    console.log(e);
                });
            }
        },
        contrastVal: {
            get: function () {
                return Math.round(this.contrast * 100);
            },
            set: function (value) {
                this.$store.commit(m.PROJECTS_SET_CONTRAST, value / 100.0);
            }
        },
        brightnessVal: {
            get: function () {
                return Math.round(this.brightness * 100);
            },
            set: function (value) {
                this.$store.commit(m.PROJECTS_SET_BRIGHTNESS, value / 100.0);
            }
        }
    },
    components: {
       PathologyImageViewer,
       AnnotationSettings,
       ClassSettings,
       NewClassSettings,
       ImgList,
       ContrastBrightnessSettings,
       ColorSettings
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
        performUndo: async function() {
            this.$store.commit(m.ANNOTATIONS_PERFORM_UNDO);
            this.$store.dispatch(a.PROJECTS_POST_ANNOTATION, {
                annotation: this.annotation,
            }).then(() => {
            }).catch((e) => {
                console.log("error fetching activeGeometry data");
                console.log(e);
            });
        },
        performRedo: async function() {
            this.$store.commit(m.ANNOTATIONS_PERFORM_REDO);
            this.$store.dispatch(a.PROJECTS_POST_ANNOTATION, {
              annotation: this.annotation,
            }).then(() => {
            }).catch((e) => {
                console.log("error fetching activeGeometry data");
                console.log(e);
            });
        },
        resetContrast: function () {
            this.$store.commit(m.PROJECTS_RESET_CONTRAST);
        },
        resetBrightnrss: function () {
            this.$store.commit(m.PROJECTS_RESET_BRIGHTNESS);
        }
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
    },
    mounted: function () {
        this.$store.dispatch(a.PROJECTS_FETCH_FILES);
        this.timer = setInterval(() => {
            this.$store.dispatch(a.PROJECTS_FETCH_FILES);
        }, 10000);
    },
    beforeDestroy: function () {
        clearInterval(this.timer);
    }
};
</script>

<style>
.toolbar-btn {
  min-width: 48px !important;
  width: 48px !important;
  height: 48px !important;
  padding: 0 12px !important;
  border: 1px solid rgba(0, 0, 0, 0.12) !important;
  color: rgba(0, 0, 0, 0.87) !important;
}
.v-navigation-drawer__border {
  display: none !important;
}
</style>
