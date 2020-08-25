<template>
  <v-dialog v-model="show" max-width="500px">
    <v-card>
      <v-card-title>
        <span class="headline">NEW CLASS SETTINGS</span>
      </v-card-title>
      <v-card-text>
        <v-container>
          <v-row>
            <v-col cols="12">
              <v-text-field
                  placeholder="class name"
                  v-model="name"
                  label="Enter Class Name">
              </v-text-field>
            </v-col>
          </v-row>
          <v-row>
            <v-col cols="12">
              <v-text-field
                  label="Stroke Color"
                  v-model="strokeColor"
                  class="ma-0 pa-0">
                <template v-slot:append>
                  <v-menu
                      v-model="strokeColorMenu"
                      top
                      nudge-bottom="105"
                      nudge-left="16"
                      :close-on-content-click="false"
                  >
                    <template v-slot:activator="{ on }">
                      <div :style="swatchStyleStroke" class="mb-2" v-on="on"/>
                    </template>
                    <v-card>
                      <v-card-text class="pa-0">
                        <v-color-picker v-model="strokeColor" flat/>
                      </v-card-text>
                    </v-card>
                  </v-menu>
                </template>
              </v-text-field>
            </v-col>
          </v-row>
          <v-row>
            <v-col cols="12">
              <v-text-field
                  label="Fill Color"
                  v-model="fillColor"
                  class="ma-0 pa-0">
                <template v-slot:append>
                  <v-menu
                      v-model="fillColorMenu"
                      top
                      nudge-bottom="105"
                      nudge-left="16"
                      :close-on-content-click="false"
                  >
                    <template v-slot:activator="{ on }">
                      <div :style="swatchStyleFill" class="mb-2" v-on="on"/>
                    </template>
                    <v-card>
                      <v-card-text class="pa-0">
                        <v-color-picker v-model="fillColor" flat/>
                      </v-card-text>
                    </v-card>
                  </v-menu>
                </template>
              </v-text-field>
            </v-col>
          </v-row>
          <!--<v-row>
              <v-col cols="12">
                  <v-card class="d-flex justify-around align-center" flat>
                      <v-slider
                          @change="$emit('input', $event)"
                          @input="$emit('input', $event)"
                          :thumb-size="24"
                          label="Opacity"
                          v-model="opacity"
                          dense
                          hide-details
                          class="ma-0"
                          min=0
                          max=100
                          >
                      </v-slider>
                      <span class="textForegroundColor--text">{{ opacity }}</span>
                  </v-card>
              </v-col>
          </v-row>-->
        </v-container>
      </v-card-text>
      <v-card-actions>
        <v-spacer></v-spacer>
        <v-btn color="blue darken-1" text @click="show = false">Cancel</v-btn>
        <v-btn :disabled="okDisabled" color="blue darken-1" text @click="onOk">OK</v-btn>
      </v-card-actions>
    </v-card>
  </v-dialog>
</template>

<script>
import * as m from "../store/mutation_types";
import * as a from "../store/action_types";
import {mapGetters} from "vuex";

export default {
  props: ["visible"],
  data: function () {
    return {
      fillColor: "#1976D2FF",
      fillColorMenu: false,
      strokeColor: "#777777FF",
      strokeColorMenu: false,
      //opacity: 100,
      mask: "!#XXXXXXXX",
      name: "",
    };
  },
  computed: {
    ...mapGetters(["classes", "activeClassName", "activeClass", "annotation"]),
    swatchStyleStroke() {
      return {
        backgroundColor: this.strokeColor,
        cursor: "pointer",
        height: "30px",
        width: "30px",
        borderRadius: this.strokeColorMenu ? "50%" : "4px",
        transition: "border-radius 200ms ease-in-out",
      };
    },
    swatchStyleFill() {
      return {
        backgroundColor: this.fillColor,
        cursor: "pointer",
        height: "30px",
        width: "30px",
        borderRadius: this.fillColorMenu ? "50%" : "4px",
        transition: "border-radius 200ms ease-in-out",
      };
    },
    show: {
      get() {
        return this.visible;
      },
      set(value) {
        if (!value) {
          this.$emit("close");
          this.clearData();
        }
      },
    },
    okDisabled: function () {
      return this.name === undefined || this.name === '';
    }
  },
  methods: {
    clearData: function () {
      this.name = '';
      this.strokeColor = "#777777FF";
      this.fillColor = "#1976D2FF";
      this.fillColorMenu = false;
      this.strokeColor = "#777777FF";
      this.strokeColorMenu = false;
      //this.opacity = 100;
    },
    onOk: async function () {
      this.$store.commit(m.PROJECTS_ADD_CLASS, {
        name: this.name,
        strokeColor: this.strokeColor,
        fillColor: this.fillColor,
        //opacity: this.opacity / 100.0,
        type: "Polygon",
      });
      this.$store.commit(m.PROJECTS_SET_ACTIVE_CLASS, {name: this.name});
      this.$store.dispatch(a.PROJECTS_POST_ANNOTATION, {
        annotation: this.annotation,
      })
          .then(() => {
          })
          .catch((e) => {
            console.log("error fetcing data");
            console.log(e);
          });
      this.show = false;
    },
  },
};
</script>
