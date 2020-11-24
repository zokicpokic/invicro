import constants from "@/utils/constants.js";
import axios from 'axios';

function removeEmptyDir (tree) {
    if (tree.type !== "dir") {
        return;
    }

    for (let i = tree.items.length - 1; i >= 0; i--) {
        if (tree.items[i].type === "dir") {
            if (!Array.isArray(tree.items[i].items) || !tree.items[i].items.length) {
                tree.items.splice(i, 1);
            }
            else {
                removeEmptyDir(tree.items[i]);
            }
        }
    }
}

function removeRoot (tree) {
    if (Array.isArray(tree.items) && tree.items.length > 0) {
        return tree.items;
    }
    return [];
}

export const files = {
    // get: async (projectId, studyId, directory) => {
    //     return axios
    //         .get(
    //             constants.API_CORE_HOST + '/api/all/' +
    //             projectId + '/' +
    //             studyId + '/' +
    //             directory)
    //         .then(response => {
    //             return response.data;
    //         })
    //         .then(response => {
    //             return JSON.parse(response.data);
    //         });
    // }
    get: async () => {
        return axios
            .get(constants.API_CORE_HOST + '/api/all/')
            .then(response => {
                return response.data;
            })
            .then(response => {
                removeEmptyDir(response);
                return response;
            })
            .then(response => {
                let arr = removeRoot(response);
                return arr;
            })
    }
};
