<template>
  <v-dialog v-model="show" max-width="500px">
    <v-card>
      <v-card-title>
        <span class="headline">CLASSES SETTINGS</span>
      </v-card-title>
      <v-card-text>
        <v-container>
          <v-row></v-row>
          <span>Active class : {{ activeClassName }}</span>
          <v-row class="flex-column">
            <v-list flat>
              <v-subheader>CLASSES</v-subheader>
              <v-list-item-group mandatory value="activeClassName" @change="onSelectionChange" color="primary">
                <class v-for="(item) in classes" :key="item.name" :className="item.name">
                </class>
              </v-list-item-group>
            </v-list>
          </v-row>
        </v-container>
      </v-card-text>
      <v-card-actions>
        <v-spacer></v-spacer>
        <v-btn color="blue darken-1" text @click="show = false">Cancel</v-btn>
        <v-btn color="blue darken-1" text @click="show = false">OK</v-btn>
      </v-card-actions>
    </v-card>
  </v-dialog>
</template>

<script>
import * as m from '../store/mutation_types';
import { mapGetters } from 'vuex';
import Class from './Class';

export default {
    props: ["visible"],
    data: () => ({

    }),
    components: {
        Class
    },
    computed: {
        ...mapGetters([
            'classes',
            'activeClassName',
            'activeClass'
            ]),
        show: {
            get() {
                return this.visible;
            },
            set(value) {
                if (!value) {
                    this.$emit("close");
                }
            },
        }
    },
    methods: {
        onSelectionChange: function (val) {
            console.log(val);
            const cl = this.classes[val];
            console.log(cl.name);
            if (cl) {
                this.$store.commit(m.PROJECTS_SET_ACTIVE_CLASS, { name: cl.name });
            }
        },
        // TODO: fix onVisibilityChange...
        onVisibilityChange: function (item) {
            this.$store.commit(m.PROJECTS_SET_CLASS_VISIBILITY, {
                name: item.name,
                isVisible: !item.isVisible
            });
        }
    }
};
</script>
