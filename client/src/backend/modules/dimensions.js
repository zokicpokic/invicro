import constants from "@/utils/constants.js";
import axios from 'axios';

export const dimensions = {
    // TODO: 1/1/stdId <--- sta je 1/1
    get: async (projectId, studyId, serieId) => {
        console.log('Dimensions call');
        return axios
            .get(
                constants.API_CORE_HOST + '/api/dimensions/' +
                projectId + '/' +
                studyId + '/' +
                serieId + '.svs/')
            .then(response => {
                return response.data;
            });
    }
};
