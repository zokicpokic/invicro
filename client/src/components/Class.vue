<template>
  <div flat
    class="d-flex justify-around align-center"
    :class="[itemActive ? 'grey': 'transparent']">
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
      max=100>
    </v-slider>-->
    <v-list-item-icon>
      <!-- TODO: move colorPicker menu to new vue component -->
      <v-menu top
        nudge-bottom="105"
        nudge-left="16"
        :close-on-content-click="false">
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
      <v-menu top
        nudge-bottom="105"
        nudge-left="16"
        :close-on-content-click="false">
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
    <v-switch label="Show" v-model="itemVisible">
      Show
    </v-switch>
    <v-dialog v-model="dialog" width="500">
      <template v-slot:activator="{ on, attrs }">
        <v-btn icon :disabled="itemActive" v-bind="attrs" v-on="on">
          <v-icon large>mdi-delete</v-icon>
        </v-btn>
      </template>

      <v-card>
        <v-card-title class="headline grey lighten-2">
          Confirm delete
        </v-card-title>
        <v-card-text>
          Are you sure you want to delete class {{ this.className }} and all its features?
        </v-card-text>
        <v-divider></v-divider>

        <v-card-actions>
          <v-spacer></v-spacer>
          <v-btn color="primary"
            text
            @click="dialog = false">
            Cancel
          </v-btn>
          <v-btn color="primary"
            text
            @click="onDeleteClass">
            OK
          </v-btn>
        </v-card-actions>
      </v-card>
    </v-dialog>
    <v-dialog v-model="dialog2" width="500">
      <template v-slot:activator="{ on, attrs }">
        <v-btn icon v-bind="attrs" v-on="on">
          <v-icon large>mdi-delete-empty-outline</v-icon>
        </v-btn>
      </template>

      <v-card>
        <v-card-title class="headline grey lighten-2">
          Confirm delete features
        </v-card-title>
        <v-card-text>
          Are you sure you want to delete all features of {{ this.className }}?
        </v-card-text>
        <v-divider></v-divider>

        <v-card-actions>
          <v-spacer></v-spacer>
          <v-btn color="primary"
              text
              @click="dialog2 = false">
            Cancel
          </v-btn>
          <v-btn color="primary"
            text
            @click="onDeleteClassFeatures">
            OK
          </v-btn>
        </v-card-actions>
      </v-card>
    </v-dialog>
  </div>
</template>

<script>
import * as m from '../store/mutation_types';
import {mapGetters} from 'vuex';

export default {
    name: 'Class',
    props: ["className"],
    data: function () {
        return {
            dialog: false,
            dialog2: false
        };
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
                this.$store.commit(m.PROJECTS_SET_ACTIVE_CLASS, {
                    name: this.className
                });
            }
        },
        itemVisible: {
            get() {
                return this.classA.isVisible;
            },
            set(value) {
                this.$store.commit(m.PROJECTS_UPDATE_CLASS, {
                    name: this.className,
                    isVisible: value
                });
            }
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
        },
        onDeleteClass: function () {
            this.$store.commit(m.PROJECTS_DELETE_CLASS, {name: this.className});
            this.dialog = false;
        },
        onDeleteClassFeatures: function () {
            this.$store.commit(m.PROJECTS_DELETE_FEATURES_OF_CLASS, {name: this.className});
            this.dialog2 = false;
        }
    }
};
</script>
