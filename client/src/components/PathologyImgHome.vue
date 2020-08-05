<!-- Copyright (c) 2020-. Authors: see NOTICE file.-->


<template>
  <div></div>
</template>

<script>
export default {
  name: "pathology-img-dashboard",
  components: {},
  async created() {
    await Promise.all(
      [
        //TO DO WAIT FOR DEFAULT FETCH MODEL
      ].map((p) => p.catch((e) => console.log(e)))
    );
  },
};
</script>

<style scoped>
td {
  vertical-align: middle !important;
}

a.project-name {
  font-weight: 600;
  font-size: 1.1rem;
}

.all-projects {
  margin-top: 1rem;
}

.right-column {
  display: flex;
  flex-direction: column;
}

.stats {
  padding-bottom: 2.5rem;
}

td:first-child {
  font-weight: 600;
  text-align: right;
  width: 25%;
}

.fas.fa-info-circle {
  cursor: pointer;
}
</style>
