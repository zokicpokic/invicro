<template>
  <v-card flat hover class="thumb-card pa-0 my-2" :class="{'thumb-card-selected' : isSelected}">
    <router-link :to="{ name: 'file', params: {projectId, studyId, serieId}}">
      <v-img contain :src="img"></v-img>
      <div class="link-text mt-2" :class="{'link-text-selected' : isSelected}">{{serieId}}</div>
    </router-link>
  </v-card>
</template>

<script>
import image from '@/assets/default_media_image.png';
import {backend} from "../backend/index";

export default {
    name: "ThumbItem",
    props: ["file"],
    data: function () {
        return {
            img: undefined
        };
    },
    computed: {
        projectId: function () {
            if (this.file) {
                return this.file.split("/")[2];
            }
            return "";
        },
        studyId: function () {
            if (this.file) {
                return this.file.split("/")[3];
            }
            return "";
        },
        serieId: function () {
            if (this.file) {
                return this.file.split("/")[4].replace(/\.[^/.]+$/, "");
            }
            return "";
        },
        isSelected: function () {
            return this.$route.params.projectId === this.projectId &&
                this.$route.params.studyId === this.studyId &&
                this.$route.params.serieId === this.serieId;
        }
    },
    mounted: function () {
        backend.thumbnail
            .get(this.file)
            .then(res => {
                if (res === undefined)
                    this.img = image;
                else
                    this.img = res;
            })
            .catch(err => {
                console.log(err);
                throw err;
            });
    }
};
</script>

<style scoped>
.thumb-card {
  width: 100%;
  background-color: transparent !important;
}
.thumb-card:hover {
  opacity: 0.6;
}
.thumb-card-selected {
  opacity: 0.6;
}
.link-text {
  text-align: center;
  color: black;
  background-color: transparent !important;
}
.link-text-selected {
  color: rgba(0,60,136,1);
}
a {
  text-decoration: none !important;
  background-color: transparent !important;
}
</style>
