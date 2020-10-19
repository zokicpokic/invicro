import constants from "@/utils/constants.js";
import axios from 'axios';

export const files = {
    get: async (projectId, studyId, directory) => {
        return axios
            .get(
                constants.API_CORE_HOST + '/api/all/' +
                projectId + '/' +
                studyId + '/' +
                directory)
            .then(response => {
                return response.data;
            })
            .then(response => {
                return JSON.parse(response.data);
            });
    }
};
