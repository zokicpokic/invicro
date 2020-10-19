/*
* Copyright (c) 2020-. Authors: see NOTICE file.
*/

import VueRouter from 'vue-router';

// Import Components
import GlobalDashboard from './components/GlobalDashboard.vue';
import PathologyImgHome from './components/PathologyImgHome.vue'
// Define routes
const routes = [
    {
        path: "/",
        component: PathologyImgHome

    },
    {
        name: 'file',
        path: '/:projectId/:studyId/:serieId',
        component: GlobalDashboard // Alternative: component: require("./components/GlobalDashboard.vue").default
    }
];

// Create router instance
const router = new VueRouter({
    routes: routes,
    linkActiveClass: 'is-active'
});

export default router;
