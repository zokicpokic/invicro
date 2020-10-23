import * as m from '../mutation_types';
import * as a from '../action_types';
import {backend} from '../../backend/index';
import Vue from 'vue';

function getEmptyAnnotation() {
    return {
        features: [],
        type: "FeatureCollection",
        classes: [],
        // activeClassName ???
    };
}

function getDefaultClass() {
    return {
        name: 'Default',
        strokeColor: '#00000077',
        fillColor: '#FFFFFF77',
        //opacity: 0.8,
        type: "Polygon",
        isVisible: true
    };
}


const state = {
    fetching: false,
    activeProjectId: undefined,
    activeStudyId: undefined,
    activeSerieId: undefined,
    activeClassName: undefined,
    activeGeometry: 'Polygon',
    mldContent: undefined,
    mldType: undefined,
    annotation: getEmptyAnnotation(),
    imageWidth: undefined,
    imageHeight: undefined,
    classChanged: false,
    files: [],
    recordedStack: [],
    currentRecordedIndex: -1
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
    mldContent: state => state.mldContent,
    mldType: state => state.mldType,
    activeClass: (state, getters) => getters.classes ? getters.classes.find(x => x.name === state.activeClassName) : undefined,
    classChanged: state => state.classChanged,
    files: state => state.files,
    canUndo: state => state.currentRecordedIndex > 0,
    canRedo: state => state.currentRecordedIndex > -1 && state.currentRecordedIndex < state.recordedStack.length - 1
};

const mutations = {
    [m.PROJECTS_SET_ACTIVE](state, {projectId, studyId, serieId}) {
        state.activeProjectId = projectId;
        state.activeStudyId = studyId;
        state.activeSerieId = serieId;
    },
    [m.PROJECTS_FETCHING](state) {
        state.fetching = true;
    },
    [m.PROJECTS_SET_MLD_CONTENT](state, res) {
        console.log('>>>> SET MLD CONTENT <<<');
        let mldContent = res;
        if (mldContent.features) {
            // TODO: move this conversion to the backend
            var maxZoomLevel =
                Math.floor(Math.max(state.imageWidth - 1, state.imageHeight - 1) / 256) +
                1;
            maxZoomLevel =
                Math.ceil(Math.log(maxZoomLevel) / Math.log(2)) + 1;
            var maxResolution = Math.pow(2, maxZoomLevel - 1);
            var maxExtention = maxResolution * 256;

            for (var i = 0; i < mldContent.features.length; i++) {
                var coordinates = mldContent.features[i].geometry.coordinates;
                if (coordinates) {
                    if (mldContent.features[i].geometry.type != "Point") {
                        for (var j = 0; j < coordinates.length; j++) {
                            var coordinates2 = coordinates[j];
                            if (coordinates2) {
                                for (var k = 0; k < coordinates2.length; k++) {
                                    var x =
                                        mldContent.features[i].geometry.coordinates[j][
                                            k
                                            ][0] *
                                        1000 +
                                        state.imageWidth / 2;
                                    x =
                                        (x * 40052752.78) / maxExtention -
                                        20026376.39;
                                    var y =
                                        state.imageHeight / 2 -
                                        mldContent.features[i].geometry.coordinates[j][
                                            k
                                            ][1] *
                                        1000;
                                    y =
                                        ((maxExtention - y) * 40097932.2) /
                                        maxExtention -
                                        20048966.1;
                                    mldContent.features[i].geometry.coordinates[j][
                                        k
                                        ][0] = x;
                                    mldContent.features[i].geometry.coordinates[j][
                                        k
                                        ][1] = y;
                                }
                            }
                        }
                    } else {
                        var x1 = mldContent.features[i].geometry.coordinates[0];
                        x1 = (x1 * 40075016.68) / maxExtention - 20037508.34;
                        var y1 = mldContent.features[i].geometry.coordinates[1];
                        y1 =
                            ((maxExtention - y1) * 40075016.68) / maxExtention -
                            20037508.34;
                        mldContent.features[i].geometry.coordinates[0] = x1;
                        mldContent.features[i].geometry.coordinates[1] = y1;
                    }
                }
                if (!mldContent.features[i].properties) {
                    mldContent.features[i].properties = {};
                }
                mldContent.features[i].properties.class = "Default";
            }
        }
        console.log('SET FINISHED <<<<');
        state.mldContent = mldContent;
        state.mldType = 'mld';
    },
    [m.PROJECTS_SET_ANNOTATION](state, annotation) {
        state.fetching = false;
        Vue.set(state, 'annotation', annotation);
        if (annotation != null) {

            if (annotation.classes == undefined) {
                annotation.classes = [];
            }

            if (annotation.classes.length == 0) {
                annotation.classes.push(getDefaultClass());
            }

            var activeClassname = (annotation.activeClassName) ? annotation.activeClassName : annotation.classes[0].name;
            if (activeClassname) {
                Vue.set(state, 'activeClassName', activeClassname);
            }
            var activeGeometry = (annotation.activeGeometry) ? annotation.activeGeometry : "Polygon";
            if (activeGeometry) {
                Vue.set(state, 'activeGeometry', activeGeometry);
            }
        }

        state.mldType = 'json';
    },
    [m.PROJECTS_SET_DIMENSIONS](state, {width, height}) {
        state.imageWidth = width;
        state.imageHeight = height;
    },
    [m.PROJECTS_ADD_CLASS](state, {name, strokeColor, fillColor}) {
        if (state.annotation === undefined)
            return;
        if (state.annotation.classes == undefined)
            return;

        if (state.currentRecordedIndex === -1) {
            //push current annotation
            state.recordedStack.push(JSON.parse(JSON.stringify(state.annotation)));
            state.currentRecordedIndex = 0;
        } else if(state.currentRecordedIndex < state.recordedStack.length -1)   {
            state.recordedStack.splice(state.currentRecordedIndex + 1, state.recordedStack.length - 1);
        }
        let index = state.annotation.classes.map(e => e.name).indexOf(name);
        if (index >= 0) {
            // if class exists, delete it
            state.annotation.classes.splice(index, 1);
        }

        state.annotation.classes.push({
            name,
            strokeColor,
            fillColor,
            //opacity,
            isVisible: true
        });

        state.recordedStack.push(JSON.parse(JSON.stringify(state.annotation)));
        state.currentRecordedIndex = state.recordedStack.length - 1;
    },
    [m.PROJECTS_UPDATE_CLASS](state, {
        name,
        strokeColor,
        fillColor,
        //opacity,
        isVisible
    }) {
        if (state.annotation === undefined)
            return;
        if (state.annotation.classes == undefined)
            return;
        if (state.currentRecordedIndex === -1) {
            //push current annotation
            state.recordedStack.push(JSON.parse(JSON.stringify(state.annotation)));
            state.currentRecordedIndex = 0;
        } else if(state.currentRecordedIndex < state.recordedStack.length -1)   {
            state.recordedStack.splice(state.currentRecordedIndex + 1, state.recordedStack.length - 1);
        }

        let index = state.annotation.classes.map(e => e.name).indexOf(name);
        if (index >= 0) {
            let cl = state.annotation.classes[index];
            cl.strokeColor = (strokeColor === undefined) ? cl.strokeColor : strokeColor;
            cl.fillColor = (fillColor === undefined) ? cl.fillColor : fillColor;
            //cl.opacity = (opacity === undefined) ? cl.opacity : opacity;
            cl.isVisible = (isVisible === undefined) ? cl.isVisible : isVisible;
        }

        state.recordedStack.push(JSON.parse(JSON.stringify(state.annotation)));
        state.currentRecordedIndex = state.recordedStack.length - 1;

        Vue.set(state, 'classChanged', !state.classChanged);
    },
    [m.PROJECTS_SET_ACTIVE_CLASS](state, {name}) {
        if (state.annotation === undefined)
            return;

        if (state.annotation.classes == undefined)
            return;
        if (state.currentRecordedIndex === -1) {
            //push current annotation
            state.recordedStack.push(JSON.parse(JSON.stringify(state.annotation)));
            state.currentRecordedIndex = 0;
        } else if(state.currentRecordedIndex < state.recordedStack.length -1)   {
            state.recordedStack.splice(state.currentRecordedIndex + 1, state.recordedStack.length - 1);
        }

        let index = state.annotation.classes.map(e => e.name).indexOf(name);
        if (index >= 0) {
            // class exists. We can set it as active
            state.activeClassName = name;
            state.annotation.activeClassName = name; // TODO: check this?
        }
        state.recordedStack.push(JSON.parse(JSON.stringify(state.annotation)));
        state.currentRecordedIndex = state.recordedStack.length - 1;

        Vue.set(state, 'classChanged', !state.classChanged);
    },
    [m.PROJECTS_DELETE_CLASS](state, {name}) {
        if (state.annotation === undefined)
            return;
        if (state.annotation.classes == undefined)
            return;

        if (state.currentRecordedIndex === -1) {
            //push current annotation
            state.recordedStack.push(JSON.parse(JSON.stringify(state.annotation)));
            state.currentRecordedIndex = 0;
        } else if(state.currentRecordedIndex < state.recordedStack.length -1)   {
            state.recordedStack.splice(state.currentRecordedIndex + 1, state.recordedStack.length - 1);
        }

        let index = state.annotation.classes.map(e => e.name).indexOf(name);
        if (index >= 0) {
            // if class exists, delete it
            state.annotation.classes.splice(index, 1);
        }

        state.annotation.features = state.annotation.features.filter(function (obj) {
            return obj.properties.class !== name;
        });

        state.recordedStack.push(JSON.parse(JSON.stringify(state.annotation)));
        state.currentRecordedIndex = state.recordedStack.length - 1;

        Vue.set(state, 'classChanged', !state.classChanged);
    },
    [m.PROJECTS_ADD_EMPTY_ANNOTATION](state) {
        state.fetching = false;
        state.annotation = getEmptyAnnotation();
    },
    [m.PROJECTS_SET_ACTIVE_GEOMETRY](state, {geometry}) {
        state.activeGeometry = geometry;
        state.annotation.activeGeometry = geometry;
    },
    [m.PROJECTS_ANNOTATION_ADD_FEATURE](state, {feature}) {
        console.log(state);
        console.log(feature);
        if (state.currentRecordedIndex == -1) {
            //push current annotation
            state.recordedStack.push(JSON.parse(JSON.stringify(state.annotation)));
            state.currentRecordedIndex = 0;
        } else if(state.currentRecordedIndex < state.recordedStack.length -1)   {
            state.recordedStack.splice(state.currentRecordedIndex + 1, state.recordedStack.length - 1);
        }
        state.annotation.features.push(feature);
        state.recordedStack.push(JSON.parse(JSON.stringify(state.annotation)));
        state.currentRecordedIndex = state.recordedStack.length - 1;
    },
    [m.PROJECTS_SET_FILES](state, files) {
        console.log(files);
        state.files = files;
    },
    [m.ANNOTATIONS_PERFORM_UNDO](state) {
        if (state.currentRecordedIndex > 0) {
            const undoAnnotation = state.recordedStack[state.currentRecordedIndex - 1];
            state.currentRecordedIndex -= 1;
            //Vue.set(state, 'annotation', null);
            Vue.set(state, 'activeGeometry', undoAnnotation.activeGeometry);
            Vue.set(state, 'activeClass', undoAnnotation.activeClass);
            Vue.set(state, 'activeClassName', undoAnnotation.activeClassName);
            Vue.set(state, 'annotation', undoAnnotation);
        }
        //Vue.set(state, 'classChanged', !state.classChanged);
    },
    [m.ANNOTATIONS_PERFORM_REDO](state) {
        if (state.currentRecordedIndex <= state.recordedStack.length - 2) {
            const redoAnnotation = state.recordedStack[state.currentRecordedIndex + 1];
            state.currentRecordedIndex += 1;
            Vue.set(state, 'activeGeometry', redoAnnotation.activeGeometry);
            Vue.set(state, 'activeClassName', redoAnnotation.activeClassName);
            Vue.set(state, 'activeClass', redoAnnotation.activeClass);
            Vue.set(state, 'annotation', redoAnnotation);
        }
    }
};

const actions = {
    [a.PROJECTS_FETCH_ANNOTATION]: async ({state, commit}) => {
        commit(m.PROJECTS_FETCHING);

        return backend.annotations.get(state.activeProjectId, state.activeStudyId, state.activeSerieId)
            .then(res => {
                if (res == undefined &&
                    res.data == undefined &&
                    res.data.features == undefined) {
                    commit(m.PROJECTS_ADD_EMPTY_ANNOTATION);
                    return;
                }

                if (res.type == 'mld') {
                    console.log('mld type set');
                    commit(m.PROJECTS_SET_MLD_CONTENT, res.data);
                } else if (res.type == 'json') {
                    console.log('json type set');
                    commit(m.PROJECTS_SET_ANNOTATION, res.data);
                }
                return res;
            })
            .catch(err => {
                commit(m.PROJECTS_ADD_EMPTY_ANNOTATION);
                throw err;
            });
    },
    [a.PROJECTS_POST_ANNOTATION]: async ({state, commit}, {annotation}) => {
        commit(m.PROJECTS_FETCHING);
        console.log('ACTION POST ANNOTATION');

        return backend.annotations
            .post(state.activeProjectId, state.activeStudyId, state.activeSerieId, annotation)
            .then(res => {
                return res;
            })
            .catch(err => {
                throw err;
            });
    },
    [a.PROJECTS_FETCH_DIMENSIONS]: async ({state, commit}) => {
        return backend.dimensions
            .get(state.activeProjectId, state.activeStudyId, state.activeSerieId)
            .then(res => {
                commit(m.PROJECTS_SET_DIMENSIONS, {width: res.width, height: res.height});
                return res;
            })
            .catch(err => {
                throw err;
            });
    },
    [a.PROJECTS_FETCH_FILES]: async ({state, commit}) => {
        console.log('ACTION POST ANNOTATION');

        return backend.files
            .get(state.activeProjectId, state.activeStudyId, "")
            .then(res => {
                commit(m.PROJECTS_SET_FILES, res);
                return res;
            })
            .catch(err => {
                console.log(err);
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
