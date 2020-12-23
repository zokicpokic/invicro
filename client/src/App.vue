<template>
  <div id="app">
    <v-app id="inspire">
      <!-- v-app-bar -->
      <v-main>
        <router-view></router-view>
      </v-main>
      <!-- v-footer -->
    </v-app>
  </div>
</template>

<script>
import * as a from "./store/action_types";

export default {
    name: "App",
    components: {},
    data: () => ({
    }),
    mounted: function () {
        this.$store.dispatch(a.CREATE_TOKEN)
            .then(() => {})
            .catch((e) => {
                console.log("error creating test token");
                console.log(e);
            });
    }
};
</script>

<style>
html, body {
  overflow: hidden !important;
}
</style>
