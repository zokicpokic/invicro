import constants from "@/utils/constants.js";
import axios from 'axios';

export const dimensions = {
    // TODO: 1/1/stdId <--- sta je 1/1
    get: async (projectId, serieId, studyId) => {
        return axios
            .get(
                constants.API_CORE_HOST + '/api/mld/dimensions/' +
                projectId + '/' +
                serieId + '/' +
                studyId + '/')
            .then(response => {
                return response.data;
            });
    }
};
