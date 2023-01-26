<?php
namespace app\controller;

use think\facade\Request;
use think\facade\Db;

class LoCyanFrpMSApp {
    public function Information(){
        $data = [
            "announcement"              => "这里是公告，还没写完qwq"
            ];
        return json($data);
    }
    public function Frpc_update(){
        if (Request::get("version") == null) { // 检查 Frpc 版本 版本对的上就不需要更新，对不上就要更新
            $data = [
                "status"                => -1,
                "message"               => "信息不完整"
                ];
            return json($data);
        } else {
            if (Request::get("version") == "V0.46.1Plus"){
                $data = [
                    "status"                => 0,
                    "needupdate"            => 0
                    ];
                return json($data);
            } else {
                $data = [
                    "status"                => 0,
                    "needupdate"            => 1,
                    "important"            => 1,
                    "version"               => "V0.46.1Plus",
                    "download_url"         => "https://download.locyan.cn/d/LoCyanFrpMSApp/LoCyanFrp/V0.46.1Plus.exe"
                    ];
                return json($data);
            }
        }
    }
    public function AuxiliaryApp_update(){
        if (Request::get("version") == null) { // 检查 Core 版本 版本对的上就不需要更新，对不上就要更新
            $data = [
                "status"                => -1,
                "message"               => "信息不完整"
                ];
            return json($data);
        } else {
            if (Request::get("version") == "V2.1"){
                $data = [
                    "status"                => 0,
                    "needupdate"            => 0
                    ];
                return json($data);
            } else {
                $data = [
                    "status"                => 0,
                    "needupdate"            => 1,
                    "version"               => "V2.1",
                    "download_url"         => "https://download.locyan.cn/d/LoCyanFrpMSApp/AuxiliaryApp/V2.1.exe"
                    ];
                return json($data);
            }
        }
    }
    public function MSApp_update(){
        if (Request::get("version") == null) { // 检查 MSApp 版本 版本对的上就不需要更新，对不上就要更新
            $data = [
                "status"                => -1,
                "message"               => "信息不完整"
                ];
            return json($data);
        } else {
            if (Request::get("version") == "Version 1.1"){
                $data = [
                    "status"                => 0,
                    "needupdate"            => 0
                    ];
                return json($data);
            } else {
                $data = [
                    "status"                => 0,
                    "important"             => 1,
                    "needupdate"            => 1,
                    "version"               => "Version 1.1",
                    "download_url"         => "https://download.locyan.cn/d/LoCyanFrpMSApp/LoCyanFrpMSApp.exe"
                    ];
                return json($data);
            }
        }
    }
    public function GetServerList(){
        $rs = Db::table("nodes")->select()->toArray();
        echo(count($rs) . " ");
        foreach ($rs as $r){
            echo($r["id"] . " " . $r["name"] . " " . $r["hostname"] . " " . $r["ip"] . " ");
        }
        return;
    }
    public function GetTunnelList(){
        $http = new \app\common\Http;
        $username = Request::get("username");
        $rs = Db::table("proxies")->where("username", $username)->select()->toArray();
        echo(count($rs) . " ");
        foreach ($rs as $r){
            // 获取主机名
            $rs = Db::table("nodes")->where("id", $r["node"])->find();
            // 是否压缩
            if ($r["use_compression"] == "true"){
                $uc = "1";
            } else {
                $uc = "0";
            }
            
            // 是否加密
            if ($r["use_encryption"] == "true"){
                $ue = "1";
            } else {
                $ue = "0";
            }
            
            // 是否存在域名
            if ($r["proxy_type"] != "http" && $r["proxy_type"] != "https"){
                $domain_S = "0";
                echo ($r["id"] . " " . $r["proxy_name"] . " " . $r["local_port"] . " " . $r["remote_port"] . " " . $r["proxy_type"] . " " . $r["node"] . " " . $uc . " " . $ue . " " . $domain_S . " " . $r["local_ip"] .  " ");
            } else {
                $domain_S = "1";
                echo ($r["id"] . " " . $r["proxy_name"] . " " . $r["local_port"] . " " . $r["remote_port"] . " " . $r["proxy_type"] . " " . $r["node"] . " " . $uc . " " . $ue . " " . $domain_S . " " . str_replace('["', "", str_replace('"]', "", $r["domain"])) . " " . $r["local_ip"] . " ");
            }
        }
        return;
    }
}