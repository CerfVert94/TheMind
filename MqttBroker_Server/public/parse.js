function parseName(url){
    let parser = document.createElement('a'),
        searchObject = {},
        queries, 
        split, 
        i,
        name;
    
    parser.href = url;

    queries = parser.search.replace(/^\?/, '').split('&');
    for( i = 0; i < queries.length; i++ ) {
        split = queries[i].split('=');
        searchObject[split[0]] = split[1];
    }
    name = searchObject['name'];
    return name
}

function parseID(topic) {
    let split   ,
        id      ;
    
    split  = topic.split('/');
    if (split[0] != "Player") {
        console.log("Wrong message");
        return -1;
    }
    id       = parseInt(split[1]);
    return id;
}
function parseSubMessage(msg) {
    let split   ,
        msg_type,
        data = []    ,
        i;

    split   = msg.split(' ');
    msg_type = split[0];
    for (i = 0; i < split.length - 1; i++) 
        data[i] = split[i + 1];
    return {msg_type, data}
}