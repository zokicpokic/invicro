<template>
    <div class="d-flex justify-around align-center" flat>
        <v-list-item-content @click="itemActive = true" :class="itemActive ? 'secondary': 'transparent'">
            <v-list-item-title v-text="className"></v-list-item-title>
        </v-list-item-content>
        <v-list-item-icon >
            <v-icon large :color="classA.strokeColor">mdi-album</v-icon>
        </v-list-item-icon>
        <v-switch label="Show"
                  v-model="itemVisible">
            Show
        </v-switch>
    </div>
</template>
<script>
import * as m from '../store/mutation_types';
import { mapGetters } from 'vuex';

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
            set () {
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
                this.$store.commit(m.PROJECTS_SET_CLASS_VISIBILITY,
                    {
                        name: this.className,
                        isVisible: value
                    });
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
        onVisibilityChange: function (item) {
            this.$store.commit(m.PROJECTS_SET_CLASS_VISIBILITY, {
                name: item.name,
                isVisible: !item.isVisible
            });
        }
    }
};

</script>
