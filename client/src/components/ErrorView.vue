<template>
    <div>
        <h1>{{errorCode}}</h1>
        <p>{{errorMsg}}</p>
    </div>
</template>

<script>
export default {
    name: 'ErrorView',
    props: ['errorCode', 'errorMsg']
};
</script>
