<template>
    <div>
        <div id="mapOL"></div>
        <!--<form class="form-inline">
            <label>Geometry type &nbsp;</label>
            <select id="type">
            <option value="Polygon">Polygon</option>
            </select>
            <br />
            <br />
            <label for="displayFeatureColor">Feature Color</label>
            <input type="color" id="displayFeatureColor" @change="featureColorChange($event)" />
            <br />
            <br />
            <input type="checkbox" id="displayMLDId" @change="check($event)" checked />
            <label for="displayMLDId">Display MLD Overlay</label>
            </form>-->
    </div>
</template>

<script>
import constants from "@/utils/constants.js";
import Map from "ol/Map.js";
import View from "ol/View.js";
import XYZ from "ol/source/XYZ";
import { Draw, Modify, Snap } from "ol/interaction";
import { Tile as TileLayer, Vector as VectorLayer } from "ol/layer";
import { Vector as VectorSource } from "ol/source";
import GeoJSON from "ol/format/GeoJSON";
import { Circle as CircleStyle, Fill, Stroke, Style } from "ol/style";
import { bbox } from "ol/loadingstrategy";
import { mapGetters } from "vuex";
import * as m from "../../store/mutation_types";
import * as a from "../../store/action_types";

//To DO:
//implement vie store
//include Image width/height as state params
export default {
    name: "pathology-image-viewer",
    data: function () {
        return {
            dataMap: null,
            dataDraw: null,
            dataSnap: null,
            featureColor: "#00000",
        };
    },
    computed: {
        ...mapGetters([
            // store getters mapped to this.<GETTER> (this.imageWidth, this.imageHeight, etc...)
            "imageWidth",
            "imageHeight",
            "annotation",
            "activeProjectId",
            "activeStudyId",
            "activeSerieId",
            "annotation",
            "activeClass",
            "activeGeometry",
            "mldContent"
        ]),
    },
    watch: {
        $route: function () {
            console.log("Route changed");
            this.loadProject();
        },
        imageWidth: function () {
            this.initView();
        },
        imageHeight: function () {
            this.initView();
        },
        mldContent: function () {
            this.updateSource();
        },
        annotation: function () {
            this.sourceAnnotations.clear();
            this.sourceAnnotations.addFeatures(
                this.sourceAnnotations
                .getFormat()
                .readFeatures(JSON.stringify(this.annotation))
            );
        }
        /*,
    activeGeometry(newValue, oldValue) {
      console.log(`Updating from ${oldValue} to ${newValue}`);
      this.loadProject();
    },*/,
    },
    mounted: function () {
        this.loadProject();
    },
    methods: {
        check: function (e) {
            var layer = this.dataMap?.getLayers().array_[1];
            layer?.setVisible(e.target.checked);
        },
        featureColorChange(e) {
            this.featureColor = e.target.value;
        },
        initView: function () {
            console.log('initView');
            var maxZoomLevel =
                Math.floor(Math.max(this.imageWidth - 1, this.imageHeight - 1) / 256) + 1;
            maxZoomLevel = Math.ceil(Math.log(maxZoomLevel) / Math.log(2)) + 1;
            var maxResolution = Math.pow(2, maxZoomLevel - 1);
            var MAX_EXTENTION = maxResolution * 256;

            //calculate restricted extent
            var extent = [
                -20026376.39,
                -20048966.1 +
                ((2 * (MAX_EXTENTION - this.imageHeight)) / MAX_EXTENTION) * 20048966.1,
                -20026376.39 + ((2 * this.imageWidth) / MAX_EXTENTION) * 20026376.39,
                20048966.1,
            ];
            this.view = new View({
                center: [
                    (((2 * this.imageWidth) / MAX_EXTENTION) * 20026376.39) / 2,
                    (((2 * (MAX_EXTENTION - this.imageHeight)) / MAX_EXTENTION) *
                        20048966.1) /
                    2,
                ],
                zoom: maxZoomLevel / 2,
                maxZoom: maxZoomLevel - 1,
                extent: extent,
            });
            this.dataMap.setView(this.view);
        },
        updateSource: function () {
            this.source.clear();
            this.source.addFeatures(
                this.source.getFormat().readFeatures(JSON.stringify(this.mldContent))
            );
        },
        loadProject: async function () {
            console.log("loadProject");
            const projectId = this.$route.params.projectId;
            const studyId = this.$route.params.studyId;
            const serieId = this.$route.params.serieId;

            this.$store.commit(m.PROJECTS_SET_ACTIVE, {
                projectId: projectId,
                studyId: studyId,
                serieId: serieId,
            });

            // TODO: dispose dataMap is not undefined if needed (check openlayers doc)

            let map = new Map({
                target: "mapOL",
                // layers: [baseLayer, vector, vectorAnnotations],
                // view: this.view,
            });
            this.dataMap = map;

            this.source = new VectorSource({
                format: new GeoJSON(),
                wrapX: false,
                strategy: bbox,
            });
            this.vector = new VectorLayer({
                name: "Vector",
                source: this.source,
                style: new Style({
                    fill: new Fill({
                        color: "rgba(255, 255, 255, 0.2)",
                    }),
                    stroke: new Stroke({
                        color: "#ffcc33",
                        width: 2,
                    }),
                    image: new CircleStyle({
                        radius: 2,
                        fill: new Fill({
                            color: "#ffcc33",
                        }),
                    }),
                }),
            });

            //use tiles on the fly
            //to do add bbox to state and return collection of tiles
            this.baseLayer = new TileLayer({
                name: "Main",
                source: new XYZ({
                    url:
                    constants.API_CORE_HOST +
                    '/api/region/' +
                    projectId + '/' +
                    studyId + '/' +
                    serieId +
                    '.svs/{z}/{x}/{y}',
                    wrapX: false,
                }),
            });

            this.sourceAnnotations = new VectorSource({
                format: new GeoJSON(),
                wrapX: false,
                strategy: bbox,
            });

            this.vectorAnnotations = new VectorLayer({
                name: "Vector",
                source: this.sourceAnnotations,
                style: styleFunction,
            });

            this.dataMap.addLayer(this.baseLayer);
            this.dataMap.addLayer(this.vector);
            this.dataMap.addLayer(this.vectorAnnotations);

            this.$store.dispatch(a.PROJECTS_FETCH_DIMENSIONS);
            this.$store.dispatch(a.PROJECTS_FETCH_ANNOTATION);
            this.$store.dispatch(a.PROJECTS_FETCH_MLD_CONTENT);

            console.log("loadProject 2");

            var styleFunction = function (feature) {
                console.log(feature);
                if (this.activeClass) {
                    var retStyle = new Style({
                        stroke: new Stroke({
                            color: this.activeClass.strokeColor,
                            width: 2,
                        }),
                        fill: new Fill({
                            color: this.activeClass.fillColor,
                        }),
                    });
                    return retStyle;
                }

                return undefined;
            };

            var modify = new Modify({ source: this.sourceAnnotations });
            console.log(modify);
            map.addInteraction(modify);
            var draw, snap; // global so we can remove them later
            //var typeSelect = document.getElementById("type");


            let activeGeometry = this.activeGeometry;
            let self = this;

            function addInteractions() {
                draw = new Draw({
                    source: self.sourceAnnotations,
                    type: activeGeometry ? activeGeometry : "Polygon",
                });
                draw.on("drawend", drawEnd);
                map.addInteraction(draw);
                snap = new Snap({ source: self.sourceAnnotations });
                map.addInteraction(snap);
                self.dataDraw = draw;
                self.dataSnap = snap;
            }

            async function drawEnd(e) {
                var writer = new GeoJSON();
                e.feature.set("class", this.activeClass.name);
                var feature = writer.writeFeature(e.feature);
                this.$store.commit(m.PROJECTS_ANNOTATION_ADD_FEATURE, {
                    feature: JSON.parse(feature),
                });
                var f = this.$store.dispatch(a.PROJECTS_POST_ANNOTATION, {
                    annotation: this.annotation,
                });

                await Promise.all([f])
                    .then(() => {})
                    .catch((e) => {
                        console.log(
                            "error fetcing PROJECTS_ANNOTATION_ADD_FEATURE data"
                        );
                        console.log(e);
                    });

                /*var writer = new GeoJSON();
            e.feature.set("class", JSON.stringify(self.activeClass));
            var features = sourceAnnotations.getFeatures();
            features = features.concat(e.feature);
            var geojsonStr = writer.writeFeatures(features);
            console.log(geojsonStr);
            var url =
              constants.API_CORE_HOST + "/api/annotations/1/1/" + serId + "/";

            axios
              .post(url, JSON.parse(geojsonStr))
              .then((response) => console.log(response));*/
            }

            /*typeSelect.onchange = function () {
            map.removeInteraction(draw);
            map.removeInteraction(snap);
            addInteractions();
          };*/

            addInteractions();
            //});



        },
    },
};
</script>
<style>
@import "~ol/ol.css";

#mapOL {
    height: 600px;
}
</style>
