<template>
  <v-menu v-model="show" :close-on-content-click="false" :nudge-width="200" offset-y>
    <template v-slot:activator="{ on, attrs }">
      <v-btn elevation="0" class="toolbar-btn" v-bind="attrs" v-on="on">
        <v-icon>{{icon}}</v-icon>
      </v-btn>
    </template>
    <v-card class="px-6" width="320">
      <v-card-title class="px-0">
        <span class="headline">{{title}}</span>
      </v-card-title>
      <v-slider min="0" max="100" v-model="sliderValue" thumb-label>
        <template v-slot:prepend>
           <v-icon @click="sliderValue--">mdi-minus</v-icon>
        </template>
        <template v-slot:append>
          <v-icon @click="sliderValue++">mdi-plus</v-icon>
          <v-spacer class="ma-2"></v-spacer>
          <v-icon @click="$emit('reset')">mdi-redo-variant</v-icon>
        </template>
      </v-slider>
    </v-card>
  </v-menu>
</template>

<script>
export default {
    name: 'ContrastBrightnessSettings',
    props: ['title', 'icon', 'value'],
    data: function () {
        return {
            show: false
        };
    },
    computed: {
        sliderValue: {
            get: function () {
                return this.value;
            },
            set: function (value) {
                this.$emit('input', value);
            }
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
  border: 1px solid rgba(0, 0, 0, 0.12) !important;
  color: rgba(0, 0, 0, 0.87) !important;
}
</style>