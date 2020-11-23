import constants from "@/utils/constants.js";
import axios from 'axios';

export const thumbnail = {
    get: async (path) => {
        return axios
            .get(constants.API_CORE_HOST + '/api/thumbnail' + path, {responseType: 'arraybuffer'})
            .then(response => {
                if (response.headers['content-type'].includes("image/png")) {
                    return "data:" + response.headers["content-type"] + ";base64," + Buffer.from(response.data, 'binary').toString('base64');
                }
                return undefined;
            });
    }
};
