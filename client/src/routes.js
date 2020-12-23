import VueRouter from 'vue-router';
import ErrorView from './components/ErrorView.vue';
import GlobalDashboard from './components/GlobalDashboard.vue';

const routes = [
    {
        name: 'main',
        path: '/',
        component: GlobalDashboard
    },
    {
        name: 'error',
        path: '/error',
        props: true,
        component: ErrorView
    }
    // {
    //     name: 'file',
    //     path: '/:projectId/:studyId/:serieId',
    //     component: GlobalDashboard
    // }
];

const router = new VueRouter({
    routes: routes,
    linkActiveClass: 'is-active'
});

export default router;
