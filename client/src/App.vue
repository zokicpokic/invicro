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
export default {
    name: "App",
    components: {},
    data: () => ({
    })
};
</script>

<style>
html, body {
  overflow: hidden !important;
}
</style>