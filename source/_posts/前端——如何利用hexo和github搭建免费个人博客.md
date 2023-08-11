---
title: 如何利用hexo和github搭建免费个人博客
date: 2019-06-11 16:42:17
tags: 
    - hexo
    - github
categories: 
    - 前端
---
文章转载来自[使用 Hexo + Github 搭建自己的博客（图文教程）](https://blog.csdn.net/qq_40147863/article/details/84942914)  
替换主题参考[Hexo 安装和替换主题、自定义博客主题](https://blog.csdn.net/qq_40147863/article/details/84946894)
<!-- more -->

- [搭建过程](#搭建过程)
  - [安装git node.js](#安装git-nodejs)
  - [新建仓库](#新建仓库)
  - [安装Hexo](#安装hexo)
  - [修改`_config.yml`](#修改_configyml)
  - [建立ssh密钥](#建立ssh密钥)
- [维护过程](#维护过程)

## 搭建过程

### 安装git node.js  
查看版本命令用  
```
$ npm -v
$ node -v
```
### 新建仓库
新建一个repository，名称为 `name.github.io`
### 安装Hexo
``` bash
$ npm install hexo -g
$ hexo -v
$ hexo init
$ npm install
$ hexo g
$ hexo s  #查看本地新建的hexo
$ npm install hexo-deployer-git --save
```
### 修改`_config.yml`
``` yml
deploy:
  type: git
  repository: git@github.com:sjl3110/sjl3110.github.io.git
  branch: master
```
### 建立ssh密钥
``` bash
ssh-keygen -t rsa -C "mail@xx.com"
```
并将默认保存位置的`id_rsa.pub`内容存放到github网站中的SSH密钥中。
## 维护过程
``` bash
$ hexo new post "blog-name"
$ hexo d -g
```