import * as m from '../mutation_types';
import * as a from '../action_types';
import { backend } from '../../backend/index';
import Vue from 'vue';

const state = {
    fetching: false,
    activeProjectId: undefined,
    activeStudyId: undefined,
    activeSerieId: undefined,
    annotation: {
        features: [],
        type: "FeatureCollection"
    },
    imageWidth: undefined,
    imageHeight: undefined
};

const getters = {
    fetching: state => state.fetching,
    activeProjectId: state => state.activeProjectId,
    activeStudyId: state => state.activeStudyId,
    activeSerieId: state => state.activeSerieId,
    annotation: state => state.annotation,
    imageWidth: state => state.imageWidth,
    imageHeight: state => state.imageHeight
};

const mutations = {
    [m.PROJECTS_SET_ACTIVE](state, { projectId, studyId, serieId }) {
        state.activeProjectId = projectId;
        state.activeStudyId = studyId;
        state.activeSerieId = serieId;
    },
    [m.PROJECTS_FETCHING](state) {
        state.fetching = true;
    },
    [m.PROJECTS_SET_ANNOTATION](state, annotation) {
        state.fetching = false;
        Vue.set(state, 'annotation', annotation);
    },
    [m.PROJECTS_SET_ANNOTATION_FAILED](state) {
        state.annotation = undefined;
        state.fetching = false;
    },
    [m.PROJECTS_SET_DIMENSIONS](state, { width, height }) {
        state.imageWidth = width;
        state.imageHeight = height;
    }
};

const actions = {
    [a.PROJECTS_FETCH_ANNOTATION]: async ({ state, commit }) => {
        commit(m.PROJECTS_FETCHING);

        return backend.annotations.get(state.activeProjectId, state.activeStudyId, state.activeSerieId)
            .then(res => {
                commit(m.PROJECTS_SET_ANNOTATION, res);
                return res;
            })
            .catch(err => {
                commit(m.PROJECTS_SET_ANNOTATION_FAILED);
                throw err;
            });
    },
    [a.PROJECTS_POST_ANNOTATION]: async ({ state, commit }, { annotation }) => {
        commit(m.PROJECTS_FETCHING);

        return backend.annotations
            .post(state.activeProjectId, state.activeSerieId, state.activeStudyId, annotation)
            .then(res => {
                return res;
            })
            .catch(err => {
                throw err;
            });
    },
    [a.PROJECTS_FETCH_DIMENSIONS]: async ({ state, commit }) => {
        return backend.dimensions
            .get(state.activeProjectId, state.activeStudyId, state.activeSerieId)
            .then(res => {
                commit(m.PROJECTS_SET_DIMENSIONS, { width: res.width, height: res.height });
                return res;
            })
            .catch(err => {
                throw err;
            });
    }
};

export default {
    state,
    actions,
    mutations,
    getters
};
