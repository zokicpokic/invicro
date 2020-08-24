import constants from "@/utils/constants.js";
import axios from 'axios';

export const annotations = {
    // TODO: 1/1/stdId <--- sta je 1/1
    get: async (projectId, studyId, serieId) => {
        return axios
            .get(
                constants.API_CORE_HOST + '/api/mld/' +
                projectId + '/' +
                studyId + '/' +
                serieId + '.svs/')
            .then(response => {
                return response.data;
            });
    },
    post: async (projectId, studyId, serieId, annotation) => {
        console.log(annotation);
        return axios
            .post(constants.API_CORE_HOST + '/api/mld/' +
                projectId + '/' +
                studyId + '/' +
                serieId + '.svs/', annotation)
            .then(response => {
                return response.data;
            });
    }
};
