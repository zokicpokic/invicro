<template>
  <v-dialog v-model="show" max-width="500px">
    <v-card>
      <v-card-title>
        <span class="headline">NEW CLASS SETTINGS</span>
      </v-card-title>
      <v-card-text>
        <v-container>
          <v-row>
            <v-col cols="12" sm="6">
              <v-text-field v-model="name" label="Enter Class Name"></v-text-field>
            </v-col>
            <v-col cols="12" sm="6">
              <v-text-field v-model="color" hide-details class="ma-0 pa-0" solo>
                <template v-slot:append>
                  <v-menu
                    v-model="menu"
                    top
                    nudge-bottom="105"
                    nudge-left="16"
                    :close-on-content-click="false"
                  >
                    <template v-slot:activator="{ on }">
                      <div :style="swatchStyle" v-on="on" />
                    </template>
                    <v-card>
                      <v-card-text class="pa-0">
                        <v-color-picker v-model="color" flat />
                      </v-card-text>
                    </v-card>
                  </v-menu>
                </template>
              </v-text-field>
            </v-col>
          </v-row>
        </v-container>
      </v-card-text>
      <v-card-actions>
        <v-spacer></v-spacer>
        <v-btn color="blue darken-1" text @click="show = false">Cancel</v-btn>
        <v-btn color="blue darken-1" text @click="onOk">OK</v-btn>
      </v-card-actions>
    </v-card>
  </v-dialog>
</template>

<script>
import * as m from "../store/mutation_types";
import * as a from "../store/action_types";
import { mapGetters } from "vuex";

export default {
  props: ["visible"],
  data: function () {
    return {
      color: "#1976D2FF",
      mask: "!#XXXXXXXX",
      name: "",
      menu: false,
    };
  },
  computed: {
    ...mapGetters(["classes", "activeClassName", "activeClass", "annotation"]),
    swatchStyle() {
      const { color, menu } = this;
      return {
        backgroundColor: color,
        cursor: "pointer",
        height: "30px",
        width: "30px",
        borderRadius: menu ? "50%" : "4px",
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
        }
      },
    },
  },
  methods: {
    onOk: async function () {
      this.$store.commit(m.PROJECTS_ADD_CLASS, {
        name: this.name,
        strokeColor: this.color,
        fillColor: "#5A5A5A", // TODO: add real values from color picker
        opacity: 0.5, // TODO: add real values
        type: "Polygon",
      });
      this.$store.commit(m.PROJECTS_SET_ACTIVE_CLASS, { name: this.name });
      var f = this.$store.dispatch(a.PROJECTS_POST_ANNOTATION, {
        annotation: this.annotation,
      });
      await Promise.all([f])
        .then(() => {})
        .catch((e) => {
          console.log("error fetcing data");
          console.log(e);
        });
      this.show = false;
    },
  },
};
</script>
