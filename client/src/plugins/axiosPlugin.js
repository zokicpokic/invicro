import axios from 'axios';

const AxiosPlugin = {};

// AxiosPlugin.install = (_Vue, _options) => {
AxiosPlugin.install = () => {
    // const store = options.store;
    // let token = options.store.getters.token;

    const token = "MTYwODY3NDk0NC42NjUzMzk3";

    axios.interceptors.request.use(config => {
        // Send cookie in request
        console.log('intercept request');
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
        return Promise.reject(error);
    });
};

export default AxiosPlugin;
