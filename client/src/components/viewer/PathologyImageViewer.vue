<template>
  <div>
    <div id="mapOL"></div>
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
      "classChanged"
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
      this.sourceAnnotations.changed();
    }
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
    }
    ,
    styleFunction: function (feature) {
      console.log(feature);
      let retStyle;
      let cls = feature.get("class");
      if (cls) {
        let currClass = this.classes.find(c => c.name == cls);
        if (currClass) {
          retStyle = currClass.isVisible ? new Style({
            stroke: new Stroke({
              color: currClass.strokeColor,
              width: 2,
            }),
            fill: new Fill({
              color: currClass.fillColor,
            }),
          }) : new Style({
            visibility: 'hidden'
          });
          return retStyle;
        }
      }
      if (this.activeClass) {
        retStyle = new Style({
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
    }
    ,
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
      });
      this.dataMap = map;

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
        style: this.styleFunction,
      });

      this.dataMap.addLayer(this.baseLayer);
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
    };*/

      this.addInteractions();
      //});


    }
    ,
  },
}
;
</script>
<style>
@import "~ol/ol.css";

#mapOL {
  height: 600px;
}
</style>
