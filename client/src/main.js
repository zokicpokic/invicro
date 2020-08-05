import Vue from 'vue'
import App from './App.vue'
import VueRouter from 'vue-router';
import router from './routes.js';
import vuetify from './plugins/vuetify';
Vue.use(VueRouter);

Vue.config.productionTip = false

new Vue({
  render: h => h(App),
  vuetify,
  router
}).$mount('#app')
