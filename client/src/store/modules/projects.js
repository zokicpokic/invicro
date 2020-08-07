import * as m from '../mutation_types';
import * as a from '../action_types';
import { backend } from '../../backend/index';
import Vue from 'vue';

const state = {
    fetching: false,
    activeProjectId: undefined,
    activeStudyId: undefined,
    activeSerieId: undefined,
    activeClassName: undefined,
    activeGeometry: 'Polygon',
    annotation: {
        features: [],
        type: "FeatureCollection",
        classes: []
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
    imageHeight: state => state.imageHeight,
    classes: state => state.annotation ? state.annotation.classes : undefined,
    geometries: state => state.geometries,
    activeGeometry: state => state.activeGeometry,
    activeClassName: state => state.activeClassName,
    activeClass: (state, getters) => getters.classes ? getters.classes.find(x => x.name === state.activeClassName) : undefined
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
    },
    [m.PROJECTS_ADD_CLASS] (state, {name, strokeColor, fillColor, opacity}) {
        if (state.annotation === undefined)
            return;
        if (state.annotation.classes == undefined)
            return;

        let index = state.annotation.classes.map(e => e.name).indexOf(name);
        if (index >= 0) {
            // if class exists, delete it
            state.annotation.classes.splice(index, 1);
        }

        state.annotation.classes.push({
            name,
            strokeColor,
            fillColor,
            opacity,
            isVisible: true
        });
    },
    [m.PROJECTS_SET_CLASS_VISIBILITY] (state, { name, isVisible }) {
        if (state.annotation === undefined)
            return;
        if (state.annotation.classes == undefined)
            return;

        let index = state.annotation.classes.map(e => e.name).indexOf(name);
        if (index >= 0) {
            // class exists. We can set it as active
            state.annotation.classes[index].isVisible = isVisible;
        }

    },
    [m.PROJECTS_SET_ACTIVE_CLASS] (state, {name}) {
        if (state.annotation === undefined)
            return;
        if (state.annotation.classes == undefined)
            return;

        let index = state.annotation.classes.map(e => e.name).indexOf(name);
        if (index >= 0) {
            // class exists. We can set it as active
            state.activeClassName = name;
        }
    },
    [m.PROJECTS_DELETE_CLASS] (state, { name }) {
        if (state.annotation === undefined)
            return;
        if (state.annotation.classes == undefined)
            return;

        let index = state.annotation.classes.map(e => e.name).indexOf(name);
        if (index >= 0) {
            // if class exists, delete it
            state.annotation.classes.splice(index, 1);
        }
    },
    [m.PROJECTS_ADD_EMPTY_ANNOTATION] (state) {
        state.fetching = false;
        state.annotation = {
            features: [],
            type: "FeatureCollection",
            classes: []
        };
    },
    [m.PROJECTS_SET_ACTIVE_GEOMETRY] (state, { geometry }) {
        state.activeGeometry = geometry;
    }
};

const actions = {
    [a.PROJECTS_FETCH_ANNOTATION]: async ({ state, commit }) => {
        commit(m.PROJECTS_FETCHING);

        return backend.annotations.get(state.activeProjectId, state.activeStudyId, state.activeSerieId)
            .then(res => {
                if (res != undefined &&
                    res.features != undefined &&
                    res.type != undefined) {
                    commit(m.PROJECTS_SET_ANNOTATION, res);
                } else {
                    commit(m.PROJECTS_ADD_EMPTY_ANNOTATION);
                }
                return res;
            })
            .catch(err => {
                commit(m.PROJECTS_SET_ANNOTATION_FAILED);
                commit(m.PROJECTS_ADD_EMPTY_ANNOTATION);
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
