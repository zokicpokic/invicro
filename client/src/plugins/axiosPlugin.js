import axios from 'axios';
import store from '../store/modules/projects';
// import router from '../routes';

const AxiosPlugin = {};

// AxiosPlugin.install = (_Vue, _options) => {
AxiosPlugin.install = () => {
    // const store = options.store;
    // let token = options.store.getters.token;

    //const token = "MTYwODY3NDk0NC42NjUzMzk3";

    axios.interceptors.request.use(config => {
        // Send cookie in request
        let token = store.state.token;

        console.log('intercept request');
        console.log(token);
        config.params = {...config.params, token}
        console.log(config.params);
        return config;
    }, error => {
        // Do something with request error
        return Promise.reject(error);
    });

    // Add a response interceptor
    axios.interceptors.response.use(response => {
        return response;
    }, error => {
        // let errCode = error.response.status + ' ' + error.response.statusText;
        // router.push({ name: 'error', params: { errorCode: errCode, errorMsg: error.response.data } });
        return Promise.reject(error);
    });
};

export default AxiosPlugin;
