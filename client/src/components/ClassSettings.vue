<template>
  <v-dialog v-model="show" max-width="500px">
    <v-card>
      <v-card-title>
        <span class="headline">CLASSES SETTINGS</span>
      </v-card-title>
      <v-card-text>
        <v-container>
          <v-row></v-row>
          <span>Active class : class1</span>
          <v-row class="flex-column">
            <v-list flat>
              <v-subheader>CLASSES</v-subheader>
              <v-list-item-group v-model="item" color="primary">
                <v-list-item v-for="(item, i) in classes" :key="i">
                  <v-list-item-content>
                    <v-list-item-title v-text="item.name"></v-list-item-title>
                  </v-list-item-content>
                  <v-list-item-icon >
                    <v-icon large :color="item.strokecolor">mdi-album</v-icon>
                  </v-list-item-icon>
                  <v-switch label="Show" color="primary" value="item.isVisible">Show</v-switch>
                </v-list-item>
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
import { mapGetters } from 'vuex';

export default {
    props: ["visible"],
    data: () => ({
        item: 0,
        items: [
            { text: "class1", icon: "mdi-album", color: "black" },
            { text: "class2", icon: "mdi-album", color: "red" },
            { text: "class3", icon: "mdi-album", color: "green" },
            { text: "MLD File", icon: "mdi-album", color: "yellow" },
        ],
    }),
    computed: {
        ...mapGetters([
            'classes',
            'activeClassName'
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
        },
    },
};
</script>
