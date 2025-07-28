import API from "./main";

export const fetchdata = async ()=> await API.get("data/");