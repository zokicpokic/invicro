<template>
  <div :class="['d-flex', 'justify-around', 'align-center', itemActive ? 'grey': 'transparent']" flat>
    <v-list-item-content @click="itemActive = true">
      <v-list-item-title v-text="className"></v-list-item-title>
    </v-list-item-content>
    <!--<v-slider
        :thumb-size="24"
        label="Opacity"
        v-model="opacity"
        dense
        hide-details
        class="ma-0"
        min=0
        max=100
        >
    </v-slider>-->
    <v-list-item-icon>
      <!-- TODO: move colorPicker menu to new vue component -->
      <v-menu
          top
          nudge-bottom="105"
          nudge-left="16"
          :close-on-content-click="false"
      >
        <template v-slot:activator="{ on }">
          <v-btn icon :color="classA.strokeColor" v-on="on">
            <v-icon large>mdi-pentagon-outline
            </v-icon>
          </v-btn>
        </template>
        <v-card>
          <v-card-text class="pa-0">
            <v-color-picker v-model="strokeColor" flat/>
          </v-card-text>
        </v-card>
      </v-menu>
    </v-list-item-icon>
    <v-list-item-icon>
      <v-menu
          top
          nudge-bottom="105"
          nudge-left="16"
          :close-on-content-click="false"
      >
        <template v-slot:activator="{ on }">
          <v-btn icon :color="classA.fillColor" v-on="on">
            <v-icon large>mdi-pentagon
            </v-icon>
          </v-btn>
        </template>
        <v-card>
          <v-card-text class="pa-0">
            <v-color-picker v-model="fillColor" flat/>
          </v-card-text>
        </v-card>
      </v-menu>
    </v-list-item-icon>
    <v-switch label="Show"
              v-model="itemVisible">
      Show
    </v-switch>
  </div>
</template>
<script>
import * as m from '../store/mutation_types';
import {mapGetters} from 'vuex';

export default {
  name: 'Class',
  props: ["className"],
  data: function () {
    return {};
  },
  computed: {
    ...mapGetters([
      'classes',
      'activeClassName'
    ]),
    classA: function () {
      return this.classes.find(x => x.name === this.className);
    },
    itemActive: {
      get() {
        return this.className === this.activeClassName;
      },
      set() {
        this.$store.commit(m.PROJECTS_SET_ACTIVE_CLASS,
            {
              name: this.className
            })
      }
    },
    itemVisible: {
      get() {
        return this.classA.isVisible;
      },
      set(value) {
        this.$store.commit(m.PROJECTS_UPDATE_CLASS,
            {
              name: this.className,
              isVisible: value
            });
      },
    },
    /*opacity: {
        get () {
            if (this.classA) {
                return this.classA.opacity * 100;
            }
            return undefined;
        },
        set (value) {
            this.$store.commit(m.PROJECTS_UPDATE_CLASS, {
                name: this.className,
                opacity: value / 100.0
            });
        }
    },*/
    strokeColor: {
      get() {
        return this.classA.strokeColor;
      },
      set(value) {
        this.$store.commit(m.PROJECTS_UPDATE_CLASS, {
          name: this.className,
          strokeColor: value
        });
      }
    },
    fillColor: {
      get() {
        return this.classA.fillColor;
      },
      set(value) {
        this.$store.commit(m.PROJECTS_UPDATE_CLASS, {
          name: this.className,
          fillColor: value
        });

      }
    }
  },
  methods: {
    onSelectionChange: function (val) {
      console.log(val);
      const cl = this.classes[val];
      console.log(cl.name);
      if (cl) {
        this.$store.commit(m.PROJECTS_SET_ACTIVE_CLASS, {name: cl.name});
      }
    }
  }
};

</script>
