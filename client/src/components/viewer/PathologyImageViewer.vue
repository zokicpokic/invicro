<template>
  <div style="width:100%; backgound-color:red" ref="mapContainer">
    <div id="mapOL"
       :style="{ height: this.mapHeight + 'px' }"
       style="background-color:#f5f5f5">
    </div>
  </div>
</template>

<script>
import constants from "@/utils/constants.js";
import Map from "ol/Map.js";
import View from "ol/View.js";
import XYZ from "ol/source/XYZ";
import {Draw, Snap} from "ol/interaction";
import {Tile as TileLayer, Vector as VectorLayer} from "ol/layer";
import {Vector as VectorSource} from "ol/source";
import GeoJSON from "ol/format/GeoJSON";
import { /* Circle as CircleStyle, */ Fill, Stroke, Style} from "ol/style";
import {bbox} from "ol/loadingstrategy";
import {mapGetters} from "vuex";
import * as m from "../../store/mutation_types";
import * as a from "../../store/action_types";
import DrawRegular from "ol-ext/interaction/DrawRegular";
import Colorize from "ol-ext/filter/Colorize";
// import RasterSource from 'ol/source/Raster';
// import ImageLayer from 'ol/layer/Image'

export default {
    name: "pathology-image-viewer",
    data: function () {
        return {
            dataMap: null,
            dataDraw: null,
            dataSnap: null,
            featureColor: "#00000",
            mapHeight: 600,
            mapWidth: 600,
            resizeObserver: null,
            baseLayer: null,
            vectorAnnotations: null,
            contrastFilter: null,
            brightnessFilter: null,
            colorFilter: null
        };
    },
    computed: {
        ...mapGetters([
            "imageWidth",
            "imageHeight",
            "annotation",
            "activeProjectId",
            "activeStudyId",
            "activeSerieId",
            "annotation",
            "activeClass",
            "activeGeometry",
            "mldContent",
            "mldType",
            "classes",
            "classChanged",
            "contrast",
            "brightness",
            "red",
            "green",
            "blue",
            "intensity"
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
        annotation: function (val) {
            console.log('annotation');
            console.log(val);
            this.sourceAnnotations.clear();
            this.sourceAnnotations.addFeatures(
                this.sourceAnnotations
                    .getFormat()
                    .readFeatures(JSON.stringify(val))
            );
            this.sourceAnnotations.changed();
        },
        activeGeometry(newValue, oldValue) {
            console.log(`Updating from ${oldValue} to ${newValue}`);
            this.addInteractions();
        },
        classChanged(newValue, oldValue) {
            console.log(`Updating from ${oldValue} to ${newValue}`);
            this.sourceAnnotations.clear();
            this.sourceAnnotations.addFeatures(
                this.sourceAnnotations
                    .getFormat()
                    .readFeatures(JSON.stringify(this.annotation))
            );
            this.sourceAnnotations.changed();
        },
        mapHeight: function () {
            if (this.dataMap) {
                this.$nextTick(() => {
                    this.dataMap.updateSize();
                    this.resizeMap();
                });
            }
        },
        contrast: function () {
            this.contrastFilter.setFilter({ operation: 'contrast', value: this.contrast});
        },
        brightness: function () {
            this.brightnessFilter.setFilter({ operation: 'luminosity', value: this.brightness});
        },
        red: function () {
            this.colorFilter.setFilter({
                operation: 'color',
                red: this.red,
                green: this.green,
                blue: this.blue,
                value: this.intensity
            })
        },
        green: function () {
            this.colorFilter.setFilter({
                operation: 'color',
                red: this.red,
                green: this.green,
                blue: this.blue,
                value: this.intensity
            })
        },
        blue: function () {
            this.colorFilter.setFilter({
                operation: 'color',
                red: this.red,
                green: this.green,
                blue: this.blue,
                value: this.intensity
            })
        },
        intensity: function () {
            this.colorFilter.setFilter({
                operation: 'color',
                red: this.red,
                green: this.green,
                blue: this.blue,
                value: this.intensity
            })
        }
    },
    mounted: function () {
        this.$nextTick(() => {
            window.addEventListener('resize', this.getWindowHeight);
            this.getWindowHeight();
        });
        this.resizeObserver = new  ResizeObserver(entries => {
            for (let entry of entries) {
                if (entry.target === this.$refs.mapContainer  && this.mapWidth !== entry.contentRect.width) {
                    this.mapWidth = entry.contentRect.width;
                    if (this.dataMap) {
                        this.$nextTick(() => {
                            this.dataMap.updateSize();
                            this.resizeMap();
                        });
                    }
                }
            }
        })
        this.resizeObserver.observe(this.$refs.mapContainer);
        this.loadProject();
    },
    methods: {
        getWindowHeight: function () {
            // body height minus menu and toolbar
            this.mapHeight = document.documentElement.clientHeight - 48 - 48;
        },
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
            let view = new View({
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
            this.dataMap.setView(view);
        },
        resizeMap: function () {
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
            let view = this.dataMap.getView();
            let newView = new View({
                center: view.getCenter(),
                zoom: view.getZoom(),
                maxZoom: maxZoomLevel - 1,
                extent: extent,
            });
            this.dataMap.setView(newView);
        },
        addInteractions: function () {
            if (this.dataDraw) {
                this.dataMap.removeInteraction(this.dataDraw);
            }
            if (this.dataSnap) {
                this.dataMap.removeInteraction(this.dataSnap);
            }
            let drawRegular;

            if (this.activeGeometry) {
                switch (this.activeGeometry) {
                    case "Polygon":
                        drawRegular = new Draw({
                            source: this.sourceAnnotations,
                            type: this.activeGeometry
                        });
                        break;
                    case "Circle":
                    case "Ellipse":
                        drawRegular = new DrawRegular({
                            source: this.sourceAnnotations,
                            sides: 0,
                            clickTolerance: 30
                        });
                        break;
                    case "Rectangle":
                    case "Square":
                        drawRegular = new DrawRegular({
                            source: this.sourceAnnotations,
                            sides: 4,
                            clickTolerance: 30
                        });
                        break;
                }
            }

            console.log(drawRegular);
            // https://openlayers.org/en/latest/apidoc/module-ol_geom_GeometryType.html
            /*let draw = new Draw({
                source: this.sourceAnnotations,
                type: this.activeGeometry ? this.activeGeometry : "Polygon",
            });*/

            let draw = drawRegular;
            draw.on("drawend", this.drawEnd);
            let snap = new Snap({source: this.sourceAnnotations});
            this.dataMap.addInteraction(draw);
            this.dataMap.addInteraction(snap);
            this.dataDraw = draw;
            this.dataSnap = snap;
        },
        drawEnd: async function (e) {
            var writer = new GeoJSON();
            e.feature.set("class", this.activeClass.name);
            var feature = writer.writeFeature(e.feature);
            this.$store.commit(m.PROJECTS_ANNOTATION_ADD_FEATURE, {
                feature: JSON.parse(feature),
            });
            this.$store.dispatch(a.PROJECTS_POST_ANNOTATION, {
                annotation: this.annotation,
            });
        },
        styleFunction: function (feature) {
            console.log(feature);
            let retStyle;
            let cls = feature.get("class");
            if (cls) {
                let currClass = this.classes.find(c => c.name == cls);
                if (currClass) {
                    retStyle = currClass.isVisible ?
                        new Style({
                            stroke: new Stroke({
                                color: currClass.strokeColor,
                                width: 2
                            }),
                            fill: new Fill({
                                color: currClass.fillColor
                            }),
                        }) :
                        new Style({
                            visibility: 'hidden'
                        });
                    return retStyle;
                }
            }
            if (this.activeClass) {
                retStyle = new Style({
                    stroke: new Stroke({
                        color: this.activeClass.strokeColor,
                        width: 2
                    }),
                    fill: new Fill({
                        color: this.activeClass.fillColor
                    })
                });
                return retStyle;
            }

            return undefined;
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

            if (this.dataMap) {
                this.dataMap.removeLayer(this.baseLayer);
                this.dataMap.removeLayer(this.vectorAnnotations);
            }
            else {
                this.dataMap = new Map({ target: "mapOL" });
            }

            //to do add bbox to state and return collection of tiles
            this.baseLayer = new TileLayer({
                name: "Main",
                source: new XYZ({
                    url: constants.API_CORE_HOST +
                        '/api/region/' +
                        projectId + '/' +
                        studyId + '/' +
                        serieId +
                        '.svs/{z}/{x}/{y}',
                    // crossOrigin: '*',
                    wrapX: false
                }),
            });

            // this.rasterSource = new RasterSource({
            //     sources: [this.baseLayer],
            //     operation: function (pixels, data) {
            //         const hcl = this.rgb2hcl(pixels[0]);
            //         let h = hcl[0] + (Math.PI * data.hue) / 180;
            //         if (h < 0) {
            //             h += twoPi;
            //         } else if (h > this.twoPi) {
            //             h -= twoPi;
            //         }
            //         hcl[0] = h;
            //         hcl[1] *= data.chroma / 100;
            //         hcl[2] *= data.lightness / 100;
            //         return this.hcl2rgb(hcl);
            //     },
            //     lib: {
            //         rgb2hcl: rgb2hcl,
            //         hcl2rgb: hcl2rgb,
            //         rgb2xyz: rgb2xyz,
            //         lab2xyz: lab2xyz,
            //         xyz2lab: xyz2lab,
            //         xyz2rgb: xyz2rgb,
            //         Xn: Xn,
            //         Yn: Yn,
            //         Zn: Zn,
            //         t0: t0,
            //         t1: t1,
            //         t2: t2,
            //         t3: t3,
            //         twoPi: twoPi
            //     },
            // });
            // var me = this;
            // this.rasterSource.on('beforeoperations', function (event) {
            //   const data = event.data;
            //   data.hue = me.hue;
            //   data.lightness = me.brightness;
            //   data.chroma = me.contrast;
            // });
            // this.rasterLayer = new ImageLayer({
            //   source: this.rasterSource,
            // });

            this.contrastFilter = new Colorize({ operation:'contrast', value: this.contrast });
            this.baseLayer.addFilter(this.contrastFilter);

            this.brightnessFilter = new Colorize({ operation:'luminosity', value: this.brightness });
            this.baseLayer.addFilter(this.brightnessFilter);

            this.colorFilter = new Colorize({
                operation: 'color',
                red: this.red,
                green: this.green,
                blue: this.blue,
                value: this.intensity
            });
            this.baseLayer.addFilter(this.colorFilter);

            this.sourceAnnotations = new VectorSource({
                format: new GeoJSON(),
                wrapX: false,
                strategy: bbox
            });

            this.vectorAnnotations = new VectorLayer({
                name: "Vector",
                source: this.sourceAnnotations,
                style: this.styleFunction
            });

            this.dataMap.addLayer(this.baseLayer);
            // this.dataMap.addLayer(this.rasterLayer);
            this.dataMap.addLayer(this.vectorAnnotations);

            this.$store.dispatch(a.PROJECTS_FETCH_DIMENSIONS);
            this.$store.dispatch(a.PROJECTS_FETCH_ANNOTATION).then(() => {
                console.log(this.mldType);
                if (this.mldType == 'mld') {
                    this.$store.dispatch(a.PROJECTS_POST_ANNOTATION, {annotation: this.mldContent}).then(() => {
                        this.$store.dispatch(a.PROJECTS_FETCH_ANNOTATION);
                    });
                }
            });

            console.log("loadProject 2");

            /* var modify = new Modify({source: this.sourceAnnotations});
            console.log(modify);
            map.addInteraction(modify); */

            // var draw, snap; // global so we can remove them later
            //var typeSelect = document.getElementById("type");


            // let activeGeometry = this.activeGeometry;
            // let self = this;

            /*typeSelect.onchange = function () {
            map.removeInteraction(draw);
            map.removeInteraction(snap);
            addInteractions();
            }; */

            this.addInteractions();
          //});
        }
    },
    beforeDestroy: function () {
        window.removeEventListener('resize', this.getWindowHeight);
        this.resizeObserver.unobserve(this.$refs.mapContainer);
    }
};
</script>

<style>
@import "~ol/ol.css";

/* #mapOL {
  height: 600px;
} */
</style>
