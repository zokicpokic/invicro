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
        <v-btn color="blue darken-1" text @click="onClose">Close</v-btn>
      </v-card-actions>
    </v-card>
  </v-dialog>
</template>

<script>
import * as m from '../store/mutation_types';
import {mapGetters} from 'vuex';
import Class from './Class';
import * as a from "@/store/action_types";

export default {
  props: ["visible"],
  data: () => ({}),
  components: {
    Class
  },
  computed: {
    ...mapGetters([
      'classes',
      'activeClassName',
      'activeClass',
      'annotation'
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
        this.$store.commit(m.PROJECTS_SET_ACTIVE_CLASS, {name: cl.name});
      }
    },
    onClose: async function () {
      this.$store.dispatch(a.PROJECTS_POST_ANNOTATION, {
        annotation: this.annotation,
      })
          .then(() => {
          })
          .catch((e) => {
            console.log("error fetching data");
            console.log(e);
          });
      this.show = false;
    }
  }
};
</script>
