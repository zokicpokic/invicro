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
import * as m from "./store/mutation_types";

export default {
    name: "App",
    components: {},
    data: () => ({
    }),
    created: function () {
        let token = this.$route.query.token;
        this.$store.commit(m.SET_TOKEN, token);
    }
};
</script>

<style>
html, body {
  overflow: hidden !important;
}
</style>
