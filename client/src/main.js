import Vue from 'vue'
import App from './App.vue'
import store from './store';
import VueRouter from 'vue-router';
import router from './routes.js';
import vuetify from './plugins/vuetify';
import constants from '@/utils/constants'

Vue.use(VueRouter);

Vue.config.productionTip = false

const getRuntimeConfig = async () => {
    //dev
    const runtimeConfig = await fetch('configuration.json');
    //prod
    //const runtimeConfig = await fetch('static/configuration.json');
    return await runtimeConfig.json()
}

getRuntimeConfig().then(function (json) {
    console.log("Runtime config: " + JSON.stringify(json));
    for (let i in constants) {
        // eslint-disable-next-line no-prototype-builtins
        if (json.hasOwnProperty(i)) {
            constants[i] = json[i];
        }
    }
    Object.freeze(constants);

    new Vue({
        render: h => h(App),
        vuetify,
        router,
        store
    }).$mount('#app')
});
