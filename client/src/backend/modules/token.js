import axios from 'axios';

const TOKEN_SERVICE = "http://192.168.1.12:5001/"

export const token = {
    post: async () => {
        return axios
            .post(TOKEN_SERVICE + '/create')
            .then(response => {
                return response.data.token;
            });
    }
};
