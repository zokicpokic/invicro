<template>
    <v-card flat hover class="thumb-card pa-0 my-2">
        <router-link :to="{ name: 'file', params: {projectId, studyId, serieId}}">
            <v-img contain :src="img"></v-img>
            <div class="link-text mt-2">{{serieId}}</div>
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
                return this.file.split("/")[0];
            }
            return "";
        },
        studyId: function () {
            if (this.file) {
                return this.file.split("/")[1];
            }
            return "";
        },
        serieId: function () {
            if (this.file) {
                return this.file.split("/")[2].replace(/\.[^/.]+$/, "");
            }
            return "";
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

<style>
.thumb-card {
    width: 100%;
}
.thumb-card:hover {
    opacity: 0.7;
}
.link-text {
    text-align: center;
    color:black;
}
a {
    text-decoration: none !important;
}
</style>