<?php
$file = $_SERVER['REQUEST_URI'];
if(strpos($file, '/gh/') === 0) {
// 过滤 非/npm/开头的文件访问
}elseif(strpos($file, '/npm/') === 0) {
// 过滤 非/wp/开头的文件访问
}elseif(strpos($file, '/wp/') === 0) {
}else{
	exit('/* 你不正常 */');
}
// 过滤 禁止php文件访问
if(strpos($file, '.php') !== false) exit('/* 你想干嘛? */');
//过滤没有文件后缀使用正则表达式
if(!preg_match('/\.\w+$/', $file)) exit('/* 你文件呢？ */');
//过滤 非静态文件访问 js css img 等
if(!preg_match('/\.(js|css|png|jpg|jpeg|gif|bmp|ico|webp|mp3|mp4|webm|ogg|svg|eot|ttf|woff|woff2|json|xml|txt|swf|htc|pdf|doc|docx|xls|xlsx|ppt|pptx|zip|rar|gz|bz2)$/i', $file)) exit('/* 文件格式不在白名单内 */');
// 过滤 非/gh/开头的文件访问
$self_path = pathinfo($_SERVER['PHP_SELF'], PATHINFO_DIRNAME); // 当前目录
$query = '';
if($_SERVER['QUERY_STRING']) // 有query string
{
    $file = explode('?', $file)[0]; // 去掉query string
    $query = '?' . $_SERVER['QUERY_STRING']; // 重新构造query string
}
$file_info = pathinfo($file); // 获取文件信息
$path = $file_info['dirname'];
if($path=='/') exit('/* ??? */');
$mimetype = get_mimetype($file_info['extension']);
$cdn_file = $file;
if(strlen($self_path)>1){ // 非根目录
   $cdn_file = substr($file, strlen($self_path));  // 去掉开头的目录
}
$local_path = substr(pathinfo( $cdn_file, PATHINFO_DIRNAME), 1); // 去掉开头的/
if($local_path && !is_dir($local_path)){ // 目录不存在
    @mkdir($local_path, 755, true); // 创建目录
}
$url = 'https://cdn.jsdelivr.net' . $cdn_file . $query; // 构造CDN文件URL
$content = curl($url); // 获取文件内容
if($content){
    header('content-type:'. $mimetype .';charset=utf-8'); // 输出文件头
    file_put_contents(substr($cdn_file, 1), $content); // 保存文件
    exit($content); // 输出文件
}else{
    header('location: ' .$url );
}


function curl($url)
{
	$ch = curl_init();
	curl_setopt($ch, CURLOPT_URL, $url);
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
	curl_setopt($ch, CURLOPT_NOSIGNAL, 1);
	curl_setopt($ch, CURLOPT_CONNECTTIMEOUT_MS, 3000);
	curl_setopt($ch, CURLOPT_TIMEOUT_MS, 3000);
	if (strpos($url, 'https') !== false) {
		curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
		curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, false);
	}
	curl_setopt($ch, CURLOPT_USERAGENT, 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/88.0.4324.190 Safari/537.36');
	$result = curl_exec($ch);
	curl_close($ch);
	return $result;
}
/**
 * 根据文件后缀获取其mine类型
 * @param string $extension
 * @return string
 */
 function get_mimetype($extension) {
	$ct['htm'] = 'text/html';
	$ct['html'] = 'text/html';
	$ct['txt'] = 'text/plain';
	$ct['asc'] = 'text/plain';
	$ct['bmp'] = 'image/bmp';
	$ct['gif'] = 'image/gif';
	$ct['jpeg'] = 'image/jpeg';
	$ct['jpg'] = 'image/jpeg';
	$ct['jpe'] = 'image/jpeg';
	$ct['png'] = 'image/png';
	$ct['ico'] = 'image/vnd.microsoft.icon';
	$ct['mpeg'] = 'video/mpeg';
	$ct['mpg'] = 'video/mpeg';
	$ct['mpe'] = 'video/mpeg';
	$ct['qt'] = 'video/quicktime';
	$ct['mov'] = 'video/quicktime';
	$ct['avi'] = 'video/x-msvideo';
	$ct['wmv'] = 'video/x-ms-wmv';
	$ct['mp2'] = 'audio/mpeg';
	$ct['mp3'] = 'audio/mpeg';
	$ct['rm'] = 'audio/x-pn-realaudio';
	$ct['ram'] = 'audio/x-pn-realaudio';
	$ct['rpm'] = 'audio/x-pn-realaudio-plugin';
	$ct['ra'] = 'audio/x-realaudio';
	$ct['wav'] = 'audio/x-wav';
	$ct['css'] = 'text/css';
	$ct['zip'] = 'application/zip';
	$ct['pdf'] = 'application/pdf';
	$ct['doc'] = 'application/msword';
	$ct['bin'] = 'application/octet-stream';
	$ct['exe'] = 'application/octet-stream';
	$ct['class'] = 'application/octet-stream';
	$ct['dll'] = 'application/octet-stream';
	$ct['xls'] = 'application/vnd.ms-excel';
	$ct['ppt'] = 'application/vnd.ms-powerpoint';
	$ct['wbxml'] = 'application/vnd.wap.wbxml';
	$ct['wmlc'] = 'application/vnd.wap.wmlc';
	$ct['wmlsc'] = 'application/vnd.wap.wmlscriptc';
	$ct['dvi'] = 'application/x-dvi';
	$ct['spl'] = 'application/x-futuresplash';
	$ct['gtar'] = 'application/x-gtar';
	$ct['gzip'] = 'application/x-gzip';
	$ct['js'] = 'application/javascript';
	$ct['swf'] = 'application/x-shockwave-flash';
	$ct['tar'] = 'application/x-tar';
    $ct['7z'] = 'application/x7zcompressed';
    $ct['rar'] = 'application/x-rar-compressed';
	$ct['xhtml'] = 'application/xhtml+xml';
	$ct['au'] = 'audio/basic';
	$ct['snd'] = 'audio/basic';
	$ct['midi'] = 'audio/midi';
	$ct['mid'] = 'audio/midi';
	$ct['m3u'] = 'audio/x-mpegurl';
	$ct['tiff'] = 'image/tiff';
	$ct['tif'] = 'image/tiff';
	$ct['rtf'] = 'text/rtf';
	$ct['wml'] = 'text/vnd.wap.wml';
	$ct['wmls'] = 'text/vnd.wap.wmlscript';
	$ct['xsl'] = 'text/xml';
	$ct['xml'] = 'text/xml';
	
	return isset($ct[strtolower($extension)]) ? $ct[strtolower($extension)] : 'text/html';
}