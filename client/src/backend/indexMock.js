import filtersData from './json/filters';
import featuresData from './json/features.json';

/**
 * Mocked fetch function.
 * It returns mockData after <time> delay.
 */
const fetch = (mockData, time = 1000) => {
    return new Promise((resolve) => {
        setTimeout(() => {
            resolve(mockData);
        }, time);
    });
};

// /**
//  * Mocked fetchFail function.
//  * It rejects promise after <time> delay.
//  *
//  */
// const fetchFail = (mockData, time = 1000) => {
//     return new Promise((_resolve, reject) => {
//         setTimeout(() => {
//             reject(mockData);
//         }, time);
//     });
// };

const filters = {
    getFilters: () => {
        return fetch(filtersData);
    }
};

const features = {
    getFeatures: function () {
        return fetch(featuresData);
    }
};

const convertor = {
};

export const backend = {
    filters,
    features,
    convertor
};
