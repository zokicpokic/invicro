<template>
  <v-menu v-model="show" :close-on-content-click="false" :nudge-width="200" offset-y>
    <template v-slot:activator="{ on, attrs }">
      <v-btn elevation="0" class="toolbar-btn" v-bind="attrs" v-on="on">
        <v-icon>mdi-palette</v-icon>
      </v-btn>
    </template>
    <v-card class="px-6" width="500">
      <v-card-title class="px-0">
        <span class="headline">COLOR SETTINGS</span>
      </v-card-title>
      <v-slider min="0" max="255" v-model="redValue" thumb-label>
        <template v-slot:prepend>
          <div style="width:90px">RED</div>
          <v-icon @click="redValue--">mdi-minus</v-icon>
        </template>
        <template v-slot:append>
          <v-icon @click="redValue++">mdi-plus</v-icon>
          <v-spacer class="ma-2"></v-spacer>
          <v-icon @click="reset('red')">mdi-redo-variant</v-icon>
        </template>
      </v-slider>
      <v-slider min="0" max="255" v-model="greenValue" thumb-label>
        <template v-slot:prepend>
          <div style="width:90px">GREEN</div>
          <v-icon @click="greenValue--">mdi-minus</v-icon>
        </template>
        <template v-slot:append>
          <v-icon @click="greenValue++">mdi-plus</v-icon>
          <v-spacer class="ma-2"></v-spacer>
          <v-icon @click="reset('green')">mdi-redo-variant</v-icon>
        </template>
      </v-slider>
      <v-slider min="0" max="255" v-model="blueValue" thumb-label>
        <template v-slot:prepend>
          <div style="width:90px">BLUE</div>
          <v-icon @click="blueValue--">mdi-minus</v-icon>
        </template>
        <template v-slot:append>
          <v-icon @click="blueValue++">mdi-plus</v-icon>
          <v-spacer class="ma-2"></v-spacer>
          <v-icon @click="reset('blue')">mdi-redo-variant</v-icon>
        </template>
      </v-slider>
      <v-slider min="0" max="100" v-model="intensityValue" thumb-label>
        <template v-slot:prepend>
          <div style="width:90px">INTENSITY</div>
          <v-icon @click="intensityValue--">mdi-minus</v-icon>
        </template>
        <template v-slot:append>
          <v-icon @click="intensityValue++">mdi-plus</v-icon>
          <v-spacer class="ma-2"></v-spacer>
          <v-icon @click="reset('intensity')">mdi-redo-variant</v-icon>
        </template>
      </v-slider>
    </v-card>
  </v-menu>
</template>

<script>
import { mapGetters } from 'vuex';
import * as m from "../store/mutation_types";

export default {
    name: 'ColorSettings',
    data: function () {
        return {
            show: false
        };
    },
    computed: {
        ...mapGetters([
            "red",
            "green",
            "blue",
            "intensity"
        ]),
        redValue: {
            get: function () {
                return this.red;
            },
            set: function (value) {
                this.$store.commit(m.PROJECTS_SET_COLOR, { color: "red", value });
            }
        },
        greenValue: {
            get: function () {
                return this.green;
            },
            set: function (value) {
                this.$store.commit(m.PROJECTS_SET_COLOR, { color: "green", value });
            }
        },
        blueValue: {
            get: function () {
                return this.blue;
            },
            set: function (value) {
                this.$store.commit(m.PROJECTS_SET_COLOR, { color: "blue", value });
            }
        },
        intensityValue: {
            get: function () {
                return Math.round(this.intensity * 100);
            },
            set: function (value) {
                this.$store.commit(m.PROJECTS_SET_COLOR, { color: "intensity", value:  value / 100.0});
            }
        }
    },
    methods: {
        reset: function (color) {
            this.$store.commit(m.PROJECTS_RESET_COLOR, color);
        }
    }
};
</script>

<style scoped>
.toolbar-btn {
  min-width: 48px !important;
  width: 48px !important;
  height: 48px !important;
  padding: 0 12px !important;
  /* border: 1px solid rgba(0, 0, 0, 0.12) !important;
  color: rgba(0, 0, 0, 0.87) !important; */
}
</style>
