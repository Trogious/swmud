--
-- PostgreSQL mud schema
--

CREATE FUNCTION backup_pdata(int) RETURNS int
    LANGUAGE plpgsql
    AS $_$
DECLARE
        pid ALIAS FOR $1;
		new_pdata_id int;
BEGIN
		-- 1
		SELECT duplicate_pdata(pid) INTO new_pdata_id;
		-- 2
		UPDATE pdata SET backed_up=current_timestamp, active='false' WHERE id=new_pdata_id;

		-- 3
		RETURN new_pdata_id; -- id backup'u
END;
$_$;

CREATE FUNCTION clone_pchar(int, int) RETURNS int
    LANGUAGE plpgsql
    AS $_$
DECLARE
        pid ALIAS FOR $1;
		clone_cut_percent ALIAS FOR $2;
		new_pchar_id int;
BEGIN
		-- 1
		SELECT duplicate_pchar(pid,'N') INTO new_pchar_id;
		-- 2
		UPDATE pchar SET cloned=current_timestamp WHERE id=new_pchar_id;
		
		-- 3 (TODO)

		-- 4
		RETURN new_pchar_id; -- id klonu
END;
$_$;

CREATE FUNCTION duplicate_pchar(int, character) RETURNS int
    LANGUAGE plpgsql
    AS $_$
DECLARE
        pid ALIAS FOR $1;
        duplicate_items ALIAS FOR $2;
		new_pchar_id int;
		old_fake_id int;
		new_fake_id int;
		tmptbl record;
		new_item_id int;
		tmptbl2 record;
BEGIN
		-- 1
        INSERT INTO tmp_pchar SELECT * FROM pchar WHERE id=pid;
		-- 2
		SELECT nextval('pchar_seq') INTO new_pchar_id;
		UPDATE tmp_pchar SET id=new_pchar_id WHERE id=pid;
		INSERT INTO pchar SELECT * FROM tmp_pchar WHERE id=new_pchar_id;
		DELETE FROM tmp_pchar WHERE id=new_pchar_id;
		-- 3, 4
		INSERT INTO tmp_language_pchar SELECT * FROM language_pchar WHERE pchar_id=pid;
		UPDATE tmp_language_pchar SET pchar_id=new_pchar_id WHERE pchar_id=pid;
		FOR tmptbl IN SELECT * FROM tmp_language_pchar WHERE pchar_id=new_pchar_id LOOP
			UPDATE tmp_language_pchar SET id=(SELECT nextval('language_pchar_seq')) WHERE pchar_id=new_pchar_id AND id=tmptbl.id;
		END LOOP;
		INSERT INTO language_pchar SELECT * FROM tmp_language_pchar WHERE pchar_id=new_pchar_id;
		DELETE FROM tmp_language_pchar WHERE pchar_id=new_pchar_id;

		INSERT INTO tmp_ability_pchar SELECT * FROM ability_pchar WHERE pchar_id=pid;
		UPDATE tmp_ability_pchar SET pchar_id=new_pchar_id WHERE pchar_id=pid;
		FOR tmptbl IN SELECT * FROM tmp_ability_pchar WHERE pchar_id=new_pchar_id LOOP
			UPDATE tmp_ability_pchar SET id=(SELECT nextval('ability_pchar_seq')) WHERE pchar_id=new_pchar_id AND id=tmptbl.id;
		END LOOP;
		INSERT INTO ability_pchar SELECT * FROM tmp_ability_pchar WHERE pchar_id=new_pchar_id;
		DELETE FROM tmp_ability_pchar WHERE pchar_id=new_pchar_id;

		INSERT INTO tmp_skill_pchar SELECT * FROM skill_pchar WHERE pchar_id=pid;
		UPDATE tmp_skill_pchar SET pchar_id=new_pchar_id WHERE pchar_id=pid;
		FOR tmptbl IN SELECT * FROM tmp_skill_pchar WHERE pchar_id=new_pchar_id LOOP
			UPDATE tmp_skill_pchar SET id=(SELECT nextval('skill_pchar_seq')) WHERE pchar_id=new_pchar_id AND id=tmptbl.id;
		END LOOP;
		INSERT INTO skill_pchar SELECT * FROM tmp_skill_pchar WHERE pchar_id=new_pchar_id;
		DELETE FROM tmp_skill_pchar WHERE pchar_id=new_pchar_id;

		INSERT INTO tmp_force_pchar SELECT * FROM force_pchar WHERE pchar_id=pid;
		UPDATE tmp_force_pchar SET pchar_id=new_pchar_id WHERE pchar_id=pid;
		FOR tmptbl IN SELECT * FROM tmp_force_pchar WHERE pchar_id=new_pchar_id LOOP
			UPDATE tmp_force_pchar SET id=(SELECT nextval('force_pchar_seq')) WHERE pchar_id=new_pchar_id AND id=tmptbl.id;
		END LOOP;
		INSERT INTO force_pchar SELECT * FROM tmp_force_pchar WHERE pchar_id=new_pchar_id;
		DELETE FROM tmp_force_pchar WHERE pchar_id=new_pchar_id;

		INSERT INTO tmp_weapon_pchar SELECT * FROM weapon_pchar WHERE pchar_id=pid;
		UPDATE tmp_weapon_pchar SET pchar_id=new_pchar_id WHERE pchar_id=pid;
		FOR tmptbl IN SELECT * FROM tmp_weapon_pchar WHERE pchar_id=new_pchar_id LOOP
			UPDATE tmp_weapon_pchar SET id=(SELECT nextval('weapon_pchar_seq')) WHERE pchar_id=new_pchar_id AND id=tmptbl.id;
		END LOOP;
		INSERT INTO weapon_pchar SELECT * FROM tmp_weapon_pchar WHERE pchar_id=new_pchar_id;
		DELETE FROM tmp_weapon_pchar WHERE pchar_id=new_pchar_id;

		INSERT INTO tmp_affect_data_pchar SELECT * FROM affect_data_pchar WHERE pchar_id=pid;
		UPDATE tmp_affect_data_pchar SET pchar_id=new_pchar_id WHERE pchar_id=pid;
		FOR tmptbl IN SELECT * FROM tmp_affect_data_pchar WHERE pchar_id=new_pchar_id LOOP
			UPDATE tmp_affect_data_pchar SET id=(SELECT nextval('affect_data_pchar_seq')) WHERE pchar_id=new_pchar_id AND id=tmptbl.id;
		END LOOP;
		INSERT INTO affect_data_pchar SELECT * FROM tmp_affect_data_pchar WHERE pchar_id=new_pchar_id;
		DELETE FROM tmp_affect_data_pchar WHERE pchar_id=new_pchar_id;

		INSERT INTO tmp_affect_pchar SELECT * FROM affect_pchar WHERE pchar_id=pid;
		UPDATE tmp_affect_pchar SET pchar_id=new_pchar_id WHERE pchar_id=pid;
		FOR tmptbl IN SELECT * FROM tmp_affect_pchar WHERE pchar_id=new_pchar_id LOOP
			UPDATE tmp_affect_pchar SET id=(SELECT nextval('affect_pchar_seq')) WHERE pchar_id=new_pchar_id AND id=tmptbl.id;
		END LOOP;
		INSERT INTO affect_pchar SELECT * FROM tmp_affect_pchar WHERE pchar_id=new_pchar_id;
		DELETE FROM tmp_affect_pchar WHERE pchar_id=new_pchar_id;

		INSERT INTO tmp_friend_pchar SELECT * FROM friend_pchar WHERE pchar_id=pid;
		UPDATE tmp_friend_pchar SET pchar_id=new_pchar_id WHERE pchar_id=pid;
		FOR tmptbl IN SELECT * FROM tmp_friend_pchar WHERE pchar_id=new_pchar_id LOOP
			UPDATE tmp_friend_pchar SET id=(SELECT nextval('friend_pchar_seq')) WHERE pchar_id=new_pchar_id AND id=tmptbl.id;
		END LOOP;
		INSERT INTO friend_pchar SELECT * FROM tmp_friend_pchar WHERE pchar_id=new_pchar_id;
		DELETE FROM tmp_friend_pchar WHERE pchar_id=new_pchar_id;

		INSERT INTO tmp_fevent_pchar SELECT * FROM fevent_pchar WHERE pchar_id=pid;
		UPDATE tmp_fevent_pchar SET pchar_id=new_pchar_id WHERE pchar_id=pid;
		FOR tmptbl IN SELECT * FROM tmp_fevent_pchar WHERE pchar_id=new_pchar_id LOOP
			UPDATE tmp_fevent_pchar SET id=(SELECT nextval('fevent_pchar_seq')) WHERE pchar_id=new_pchar_id AND id=tmptbl.id;
		END LOOP;
		INSERT INTO fevent_pchar SELECT * FROM tmp_fevent_pchar WHERE pchar_id=new_pchar_id;
		DELETE FROM tmp_fevent_pchar WHERE pchar_id=new_pchar_id;

		INSERT INTO tmp_quest_done_pchar SELECT * FROM quest_done_pchar WHERE pchar_id=pid;
		UPDATE tmp_quest_done_pchar SET pchar_id=new_pchar_id WHERE pchar_id=pid;
		FOR tmptbl IN SELECT * FROM tmp_quest_done_pchar WHERE pchar_id=new_pchar_id LOOP
			UPDATE tmp_quest_done_pchar SET id=(SELECT nextval('quest_done_pchar_seq')) WHERE pchar_id=new_pchar_id AND id=tmptbl.id;
		END LOOP;
		INSERT INTO quest_done_pchar SELECT * FROM tmp_quest_done_pchar WHERE pchar_id=new_pchar_id;
		DELETE FROM tmp_quest_done_pchar WHERE pchar_id=new_pchar_id;

		INSERT INTO tmp_killtrack_pchar SELECT * FROM killtrack_pchar WHERE pchar_id=pid;
		UPDATE tmp_killtrack_pchar SET pchar_id=new_pchar_id WHERE pchar_id=pid;
		FOR tmptbl IN SELECT * FROM tmp_killtrack_pchar WHERE pchar_id=new_pchar_id LOOP
			UPDATE tmp_killtrack_pchar SET id=(SELECT nextval('killtrack_pchar_seq')) WHERE pchar_id=new_pchar_id AND id=tmptbl.id;
		END LOOP;
		INSERT INTO killtrack_pchar SELECT * FROM tmp_killtrack_pchar WHERE pchar_id=new_pchar_id;
		DELETE FROM tmp_killtrack_pchar WHERE pchar_id=new_pchar_id;

		SELECT fake_id INTO old_fake_id FROM pchar WHERE id=pid;
		IF old_fake_id IS NOT NULL THEN
			INSERT INTO tmp_fake_pchar SELECT * FROM fake_pchar WHERE id=old_fake_id;
			SELECT nextval('fake_pchar_seq') INTO new_fake_id;
			UPDATE tmp_fake_pchar SET id=new_fake_id WHERE id=old_fake_id;
			UPDATE pchar SET fake_id=new_fake_id WHERE id=new_pchar_id;
			INSERT INTO fake_pchar SELECT * FROM tmp_fake_pchar WHERE id=new_fake_id;
			DELETE FROM tmp_fake_pchar WHERE id=new_fake_id;
		END IF;

		IF upper(duplicate_items)='T' THEN
			INSERT INTO tmp_item_pchar SELECT * FROM item_pchar WHERE pchar_id=pid;
			UPDATE tmp_item_pchar SET pchar_id=new_pchar_id WHERE pchar_id=pid;
			FOR tmptbl IN SELECT * FROM tmp_item_pchar WHERE pchar_id=new_pchar_id LOOP
				SELECT nextval('item_pchar_seq') INTO new_item_id;
				UPDATE tmp_item_pchar SET id=new_item_id WHERE pchar_id=new_pchar_id AND id=tmptbl.id;
				INSERT INTO item_pchar SELECT * FROM tmp_item_pchar WHERE id=new_item_id AND pchar_id=new_pchar_id;

				INSERT INTO tmp_affect_data_item_pchar SELECT * FROM affect_data_item_pchar WHERE item_id=tmptbl.id;
				UPDATE tmp_affect_data_item_pchar SET item_id=new_item_id WHERE item_id=tmptbl.id;
				FOR tmptbl2 IN SELECT * FROM tmp_affect_data_item_pchar WHERE item_id=new_item_id LOOP
					UPDATE tmp_affect_data_item_pchar SET id=(SELECT nextval('affect_data_item_pchar_seq')) WHERE item_id=new_item_id AND id=tmptbl2.id;
				END LOOP;
				INSERT INTO affect_data_item_pchar SELECT * FROM tmp_affect_data_item_pchar WHERE item_id=new_item_id;
				DELETE FROM tmp_affect_data_item_pchar WHERE item_id=new_item_id;

				INSERT INTO tmp_affect_item_pchar SELECT * FROM affect_item_pchar WHERE item_id=tmptbl.id;
				UPDATE tmp_affect_item_pchar SET item_id=new_item_id WHERE item_id=tmptbl.id;
				FOR tmptbl2 IN SELECT * FROM tmp_affect_item_pchar WHERE item_id=new_item_id LOOP
					UPDATE tmp_affect_item_pchar SET id=(SELECT nextval('affect_item_pchar_seq')) WHERE item_id=new_item_id AND id=tmptbl2.id;
				END LOOP;
				INSERT INTO affect_item_pchar SELECT * FROM tmp_affect_item_pchar WHERE item_id=new_item_id;
				DELETE FROM tmp_affect_item_pchar WHERE item_id=new_item_id;

				INSERT INTO tmp_extradescription_item_pchar SELECT * FROM extradescription_item_pchar WHERE item_id=tmptbl.id;
				UPDATE tmp_extradescription_item_pchar SET item_id=new_item_id WHERE item_id=tmptbl.id;
				FOR tmptbl2 IN SELECT * FROM tmp_extradescription_item_pchar WHERE item_id=new_item_id LOOP
					UPDATE tmp_extradescription_item_pchar SET id=(SELECT nextval('extradescription_item_pchar_seq')) WHERE item_id=new_item_id AND id=tmptbl2.id;
				END LOOP;
				INSERT INTO extradescription_item_pchar SELECT * FROM tmp_extradescription_item_pchar WHERE item_id=new_item_id;
				DELETE FROM tmp_extradescription_item_pchar WHERE item_id=new_item_id;
			END LOOP;
			DELETE FROM tmp_item_pchar WHERE pchar_id=new_pchar_id;
		END IF;

		-- 5
		RETURN new_pchar_id; -- id kopii
END;
$_$;

CREATE FUNCTION duplicate_pdata(int) RETURNS int
    LANGUAGE plpgsql
    AS $_$
DECLARE
        pid ALIAS FOR $1;
		new_pdata_id int;
		new_pchar_id int;
		new_item_id int;
		new_home_id int;
		tmptbl record;
		tmptbl2 record;
		tmptbl3 record;
BEGIN
		-- 1
        INSERT INTO tmp_pdata SELECT * FROM pdata WHERE id=pid;
		-- 2
		SELECT nextval('pdata_seq') INTO new_pdata_id;
		UPDATE tmp_pdata SET id=new_pdata_id WHERE id=pid;
		INSERT INTO pdata SELECT * FROM tmp_pdata WHERE id=new_pdata_id;
		DELETE FROM tmp_pdata WHERE id=new_pdata_id;
		-- 3, 4
		FOR tmptbl IN SELECT * FROM pchar WHERE pdata_id=pid LOOP
			SELECT duplicate_pchar(tmptbl.id,'T') INTO new_pchar_id;
			UPDATE pchar SET pdata_id=new_pdata_id WHERE id=new_pchar_id;
		END LOOP;

		INSERT INTO tmp_forbid_pdata SELECT * FROM forbid_pdata WHERE pdata_id=pid;
		UPDATE tmp_forbid_pdata SET pdata_id=new_pdata_id WHERE pdata_id=pid;
		FOR tmptbl IN SELECT * FROM tmp_forbid_pdata WHERE pdata_id=new_pdata_id LOOP
			UPDATE tmp_forbid_pdata SET id=(SELECT nextval('forbid_pdata_seq')) WHERE pdata_id=new_pdata_id AND id=tmptbl.id;
		END LOOP;
		INSERT INTO forbid_pdata SELECT * FROM tmp_forbid_pdata WHERE pdata_id=new_pdata_id;
		DELETE FROM tmp_forbid_pdata WHERE pdata_id=new_pdata_id;

		INSERT INTO tmp_bestow_pdata SELECT * FROM bestow_pdata WHERE pdata_id=pid;
		UPDATE tmp_bestow_pdata SET pdata_id=new_pdata_id WHERE pdata_id=pid;
		FOR tmptbl IN SELECT * FROM tmp_bestow_pdata WHERE pdata_id=new_pdata_id LOOP
			UPDATE tmp_bestow_pdata SET id=(SELECT nextval('bestow_pdata_seq')) WHERE pdata_id=new_pdata_id AND id=tmptbl.id;
		END LOOP;
		INSERT INTO bestow_pdata SELECT * FROM tmp_bestow_pdata WHERE pdata_id=new_pdata_id;
		DELETE FROM tmp_bestow_pdata WHERE pdata_id=new_pdata_id;

		INSERT INTO tmp_home_pdata SELECT * FROM home_pdata WHERE pdata_id=pid;
		UPDATE tmp_home_pdata SET pdata_id=new_pdata_id WHERE pdata_id=pid;
		FOR tmptbl IN SELECT * FROM tmp_home_pdata WHERE pdata_id=new_pdata_id LOOP
			SELECT nextval('home_pdata_seq') INTO new_home_id;
			UPDATE tmp_home_pdata SET id=new_home_id WHERE pdata_id=new_pdata_id AND id=tmptbl.id;
			INSERT INTO home_pdata SELECT * FROM tmp_home_pdata WHERE pdata_id=new_pdata_id;

			INSERT INTO tmp_item_home SELECT * FROM item_home WHERE home_id=tmptbl.id;
			UPDATE tmp_item_home SET home_id=new_home_id WHERE home_id=tmptbl.id;
			FOR tmptbl2 IN SELECT * FROM tmp_item_home WHERE home_id=new_home_id LOOP
				SELECT nextval('item_home_seq') INTO new_item_id;
				UPDATE tmp_item_home SET id=new_item_id WHERE home_id=new_home_id AND id=tmptbl2.id;
				INSERT INTO item_home SELECT * FROM tmp_item_home WHERE id=new_item_id AND home_id=new_home_id;

				INSERT INTO tmp_affect_data_item_home SELECT * FROM affect_data_item_home WHERE item_id=tmptbl2.id;
				UPDATE tmp_affect_data_item_home SET item_id=new_item_id WHERE item_id=tmptbl2.id;
				FOR tmptbl3 IN SELECT * FROM tmp_affect_data_item_home WHERE item_id=new_item_id LOOP
					UPDATE tmp_affect_data_item_home SET id=(SELECT nextval('affect_data_item_home_seq')) WHERE item_id=new_item_id AND id=tmptbl3.id;
				END LOOP;
				INSERT INTO affect_data_item_home SELECT * FROM tmp_affect_data_item_home WHERE item_id=new_item_id;
				DELETE FROM tmp_affect_data_item_home WHERE item_id=new_item_id;

				INSERT INTO tmp_affect_item_home SELECT * FROM affect_item_home WHERE item_id=tmptbl2.id;
				UPDATE tmp_affect_item_home SET item_id=new_item_id WHERE item_id=tmptbl2.id;
				FOR tmptbl3 IN SELECT * FROM tmp_affect_item_home WHERE item_id=new_item_id LOOP
					UPDATE tmp_affect_item_home SET id=(SELECT nextval('affect_item_home_seq')) WHERE item_id=new_item_id AND id=tmptbl3.id;
				END LOOP;
				INSERT INTO affect_item_home SELECT * FROM tmp_affect_item_home WHERE item_id=new_item_id;
				DELETE FROM tmp_affect_item_home WHERE item_id=new_item_id;

				INSERT INTO tmp_extradescription_item_home SELECT * FROM extradescription_item_home WHERE item_id=tmptbl2.id;
				UPDATE tmp_extradescription_item_home SET item_id=new_item_id WHERE item_id=tmptbl2.id;
				FOR tmptbl3 IN SELECT * FROM tmp_extradescription_item_home WHERE item_id=new_item_id LOOP
					UPDATE tmp_extradescription_item_home SET id=(SELECT nextval('extradescription_item_home_seq')) WHERE item_id=new_item_id AND id=tmptbl3.id;
				END LOOP;
				INSERT INTO extradescription_item_home SELECT * FROM tmp_extradescription_item_home WHERE item_id=new_item_id;
				DELETE FROM tmp_extradescription_item_home WHERE item_id=new_item_id;
			END LOOP;
			DELETE FROM tmp_item_home WHERE home_id=new_home_id;
		END LOOP;
		DELETE FROM tmp_home_pdata WHERE pdata_id=new_pdata_id;

		-- 5
		RETURN new_pdata_id; -- id kopii
END;
$_$;

CREATE FUNCTION pazaak_add_player(varchar, varchar) RETURNS int
    LANGUAGE plpgsql
    AS $_$
DECLARE
	pname ALIAS FOR $1;
	ppassword ALIAS FOR $2;
	pid int;
BEGIN
	SELECT id INTO pid FROM pazaak_player WHERE lower(name) = lower(pname);
	IF pid IS NULL THEN
		INSERT INTO pazaak_player(name,password) VALUES(pname,ppassword);
		SELECT last_value INTO pid FROM pazaak_player_seq;
	ELSE
		UPDATE pazaak_player SET password=ppassword WHERE id=pid;
	END IF;
	RETURN pid;
END;
$_$;

CREATE FUNCTION pazaak_finalize_game() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
DECLARE
	win record;
BEGIN
	win := NEW;
	RAISE NOTICE 'win.winner_id: %',win.winner_id;
	-- winner_id IS NULL = pazaak_game is finished
	IF win.winner_id IS NOT NULL THEN
		UPDATE pazaak_player SET credits=(SELECT credits FROM pazaak_player pp WHERE pp.id=win.winner_id)+win.reward WHERE id=win.winner_id;
		RAISE NOTICE 'win.winner_id: %',win.winner_id;
		RAISE NOTICE 'win.finished: %',win.finished;
	END IF;
	RETURN win;
END;
$$;

CREATE FUNCTION pazaak_withdraw_credits(varchar) RETURNS int
    LANGUAGE plpgsql
    AS $_$
DECLARE
	pname ALIAS FOR $1;
	pid int;
	pcredits int;
BEGIN
	SELECT id,credits INTO pid,pcredits FROM pazaak_player WHERE lower(name) = lower(pname);
	IF pcredits IS NULL THEN
		pcredits := 0;
	ELSE
		UPDATE pazaak_player SET credits=0 WHERE id=pid;
	END IF;
	RETURN pcredits;
END;
$_$;

CREATE FUNCTION restore_pdata(varchar) RETURNS int
    LANGUAGE plpgsql
    AS $_$
DECLARE
        pname ALIAS FOR $1;
		old_pdata_id int;
BEGIN
		-- 1
		SELECT id FROM pdata WHERE name=pname INTO old_pdata_id ORDER BY backed_up DESC NULLS LAST LIMIT 1;
		-- 2
		UPDATE pdata SET active='true' WHERE id=old_pdata_id;

		-- 3
		RETURN old_pdata_id; -- id przywroconego backup'u
END;
$_$;

CREATE SEQUENCE ability_pchar_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;

CREATE TABLE ability_pchar (
    id int DEFAULT nextval('ability_pchar_seq') NOT NULL UNIQUE,
    no smallint NOT NULL,
    level smallint DEFAULT 1 NOT NULL,
    experience bigint DEFAULT 0 NOT NULL,
    pchar_id int NOT NULL,
    PRIMARY KEY(no,pchar_id),
	FOREIGN KEY(pchar_id) REFERENCES pchar(id) ON DELETE CASCADE
);

CREATE TABLE generic_affect_data (
    duration int DEFAULT 0 NOT NULL,
    modifier int DEFAULT 0 NOT NULL,
    location int DEFAULT 0 NOT NULL,
    bitvector bigint DEFAULT 0 NOT NULL,
    name varchar(32) NOT NULL
);

CREATE SEQUENCE affect_data_item_home_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;

CREATE TABLE affect_data_item_home (
    id int DEFAULT nextval('affect_data_item_home_seq') NOT NULL,
    item_id int NOT NULL
)
INHERITS (generic_affect_data);

CREATE SEQUENCE affect_data_item_pchar_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;

CREATE TABLE affect_data_item_pchar (
    id int DEFAULT nextval('affect_data_item_pchar_seq') NOT NULL,
    item_id int NOT NULL
)
INHERITS (generic_affect_data);

CREATE SEQUENCE affect_data_pchar_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;

CREATE TABLE affect_data_pchar (
    id int DEFAULT nextval('affect_data_pchar_seq') NOT NULL,
    pchar_id int NOT NULL
)
INHERITS (generic_affect_data);

CREATE SEQUENCE affect_item_home_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;

CREATE TABLE generic_affect (
    duration int DEFAULT 0 NOT NULL,
    modifier int DEFAULT 0 NOT NULL,
    location int DEFAULT 0 NOT NULL,
    bitvector bigint DEFAULT 0 NOT NULL,
    type int NOT NULL
);

CREATE TABLE affect_item_home (
    id int DEFAULT nextval('affect_item_home_seq') NOT NULL,
    item_id int NOT NULL
)
INHERITS (generic_affect);

CREATE SEQUENCE affect_item_pchar_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;

CREATE TABLE affect_item_pchar (
    id int DEFAULT nextval('affect_item_pchar_seq') NOT NULL,
    item_id int NOT NULL
)
INHERITS (generic_affect);

CREATE SEQUENCE affect_pchar_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;

CREATE TABLE affect_pchar (
    id int DEFAULT nextval('affect_pchar_seq') NOT NULL,
    pchar_id int NOT NULL
)
INHERITS (generic_affect);

CREATE SEQUENCE bestow_pdata_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;

ALTER TABLE bestow_pdata_seq OWNER TO mud;

CREATE TABLE generic_cmd_pdata (
    pdata_id int NOT NULL,
    cmd varchar(32) NOT NULL
);

CREATE TABLE bestow_pdata (
    id int DEFAULT nextval('bestow_pdata_seq') NOT NULL
)
INHERITS (generic_cmd_pdata);

CREATE SEQUENCE bounty_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;

CREATE TABLE bounty (
    id int DEFAULT nextval('bounty_seq') NOT NULL,
    class varchar(32) NOT NULL,
    reward bigint NOT NULL,
    placed_by varchar(32) NOT NULL,
    hold_by bigint NOT NULL,
    wanted varchar(32) NOT NULL
);

CREATE SEQUENCE extradescription_item_home_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;

CREATE TABLE extradescription_item_home (
    id int DEFAULT nextval('extradescription_item_home_seq') NOT NULL,
    item_id int NOT NULL,
    keyword varchar(32) NOT NULL,
    description text NOT NULL
);

CREATE SEQUENCE extradescription_item_pchar_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;

CREATE TABLE extradescription_item_pchar (
    id int DEFAULT nextval('extradescription_item_pchar_seq') NOT NULL,
    item_id int NOT NULL,
    keyword varchar(32) NOT NULL,
    description text NOT NULL
);

CREATE SEQUENCE fake_pchar_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;

CREATE TABLE fake_pchar (
    id int DEFAULT nextval('fake_pchar_seq') NOT NULL,
    name varchar(16),
    inflect varchar(16)[],
    title varchar(32),
    long varchar(128),
    "desc" text
);

CREATE SEQUENCE fevent_pchar_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;

CREATE TABLE fevent_pchar (
    id int DEFAULT nextval('fevent_pchar_seq') NOT NULL,
    trigger smallint NOT NULL,
    string_attr varchar(32) DEFAULT '' NOT NULL,
    attributes bigint[] DEFAULT '{0,0,0,0,0}'::bigint[] NOT NULL,
    pchar_id int
);

CREATE SEQUENCE forbid_pdata_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;

CREATE TABLE forbid_pdata (
    id int DEFAULT nextval('forbid_pdata_seq') NOT NULL
)
INHERITS (generic_cmd_pdata);

CREATE SEQUENCE force_pchar_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;

CREATE TABLE generic_skill_pchar (
    name varchar(32) NOT NULL,
    learned smallint DEFAULT 0 NOT NULL,
    pchar_id int NOT NULL
);

CREATE TABLE force_pchar (
    id int DEFAULT nextval('force_pchar_seq') NOT NULL
)
INHERITS (generic_skill_pchar);

CREATE SEQUENCE friend_pchar_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;

CREATE TABLE friend_pchar (
    id int DEFAULT nextval('friend_pchar_seq'),
    friend_id int NOT NULL,
    pchar_id int NOT NULL
);

CREATE TABLE generic_item (
    vnum bigint NOT NULL,
    container_vnum bigint,
    count smallint,
    name varchar(128),
    inflect varchar(64)[],
    description text,
    action_desc text,
    owner varchar(16),
    in_room bigint,
    extra_flags bigint,
    wear_flags bigint,
    wear_loc smallint,
    item_type smallint,
    weight smallint,
    level smallint,
    timer int,
    cost int,
    gender smallint,
    value smallint[],
    str_value varchar(32)[],
    in_quest bigint
);

CREATE SEQUENCE generic_skill_pchar_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;

CREATE SEQUENCE home_pdata_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;

CREATE TABLE home_pdata (
    id int DEFAULT nextval('home_pdata_seq') NOT NULL,
    pdata_id int NOT NULL,
    vnum bigint DEFAULT 0 NOT NULL
);

CREATE SEQUENCE item_home_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;

CREATE TABLE item_home (
    id int DEFAULT nextval('item_home_seq') NOT NULL,
    home_id int
)
INHERITS (generic_item);

CREATE SEQUENCE item_pchar_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;

CREATE TABLE item_pchar (
    id int DEFAULT nextval('item_pchar_seq') NOT NULL,
    pchar_id int
)
INHERITS (generic_item);

CREATE SEQUENCE killtrack_pchar_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;

CREATE TABLE killtrack_pchar (
    id int DEFAULT nextval('killtrack_pchar_seq'),
    vnum int NOT NULL,
    killed int DEFAULT 0 NOT NULL,
    pchar_id int NOT NULL
);

CREATE SEQUENCE language_pchar_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;

CREATE TABLE language_pchar (
    id int DEFAULT nextval('language_pchar_seq') NOT NULL
)
INHERITS (generic_skill_pchar);

CREATE SEQUENCE pazaak_card_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;

CREATE TABLE pazaak_card (
    id int DEFAULT nextval('pazaak_card_seq') NOT NULL,
    player_id int NOT NULL,
    type smallint NOT NULL,
    sign smallint DEFAULT 0 NOT NULL,
    double_signed smallint DEFAULT 0 NOT NULL,
    CONSTRAINT pazaak_card_double_signed_check CHECK (((double_signed = 0) OR (double_signed = 1)))
);

CREATE SEQUENCE pazaak_game_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;

CREATE TABLE pazaak_game (
    id int DEFAULT nextval('pazaak_game_seq') NOT NULL,
    aggressor_id int NOT NULL,
    defender_id int NOT NULL,
    reward bigint NOT NULL,
    challenged timestamp DEFAULT now() NOT NULL,
    winner_id int,
    finished timestamp,
    CONSTRAINT pazaak_game_check CHECK (((winner_id = aggressor_id) OR (winner_id = defender_id)))
);

CREATE SEQUENCE pazaak_player_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;

CREATE TABLE pazaak_player (
    id int DEFAULT nextval('pazaak_player_seq') NOT NULL,
    name varchar(32) NOT NULL,
    password varchar(32) NOT NULL,
    credits bigint DEFAULT 0 NOT NULL
);

CREATE SEQUENCE pchar_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;

CREATE TABLE pchar (
    id int DEFAULT nextval('pchar_seq') NOT NULL,
    pdata_id int NOT NULL,
    inflect varchar(16)[] NOT NULL,
    long_descr varchar(128) NOT NULL,
    description text NOT NULL,
    main_ability smallint NOT NULL,
    speaking int DEFAULT 0 NOT NULL,
    top_level smallint DEFAULT 0 NOT NULL,
    practice smallint DEFAULT 0 NOT NULL,
    room int NOT NULL,
    hit smallint DEFAULT 0 NOT NULL,
    max_hit smallint DEFAULT 1 NOT NULL,
    move smallint DEFAULT 0 NOT NULL,
    max_move smallint DEFAULT 1 NOT NULL,
    perm_frc smallint DEFAULT 0 NOT NULL,
    mod_frc smallint DEFAULT 0 NOT NULL,
    mana smallint DEFAULT 0 NOT NULL,
    max_mana smallint DEFAULT 0 NOT NULL,
    credits int DEFAULT 0 NOT NULL,
    affected_by bigint DEFAULT 0 NOT NULL,
    fake_id int,
    "position" smallint NOT NULL,
    saving_poison_death int,
    saving_wand int,
    saving_para_petri int,
    saving_breath int,
    saving_spell_staff int,
    alignment smallint DEFAULT 0 NOT NULL,
    quest_points smallint DEFAULT 0 NOT NULL,
    hitroll smallint DEFAULT 0 NOT NULL,
    damroll smallint DEFAULT 0 NOT NULL,
    armor smallint DEFAULT 0 NOT NULL,
    wimpy smallint DEFAULT 0 NOT NULL,
    resistant bigint DEFAULT 0 NOT NULL,
    immune bigint DEFAULT 0 NOT NULL,
    susceptible bigint DEFAULT 0 NOT NULL,
    mental_state smallint DEFAULT '-10'::int NOT NULL,
    bio text DEFAULT ''::text NOT NULL,
    addiction smallint[] DEFAULT '{0,0,0,0,0,0,0,0,0}' NOT NULL,
    drug_level smallint[] DEFAULT '{0,0,0,0,0,0,0,0,0}' NOT NULL,
    perm_str smallint DEFAULT 1 NOT NULL,
    perm_int smallint DEFAULT 1 NOT NULL,
    perm_wis smallint DEFAULT 1 NOT NULL,
    perm_dex smallint DEFAULT 1 NOT NULL,
    perm_con smallint DEFAULT 1 NOT NULL,
    perm_cha smallint DEFAULT 1 NOT NULL,
    perm_lck smallint DEFAULT 0 NOT NULL,
    mod_str smallint DEFAULT 1 NOT NULL,
    mod_int smallint DEFAULT 1 NOT NULL,
    mod_wis smallint DEFAULT 1 NOT NULL,
    mod_dex smallint DEFAULT 1 NOT NULL,
    mod_con smallint DEFAULT 1 NOT NULL,
    mod_cha smallint DEFAULT 1 NOT NULL,
    mod_lck smallint DEFAULT 0 NOT NULL,
    condition smallint[] DEFAULT '{0,0,0,0}' NOT NULL,
    cloned timestamp
);

CREATE SEQUENCE pdata_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;

CREATE TABLE pdata (
    id int DEFAULT nextval('pdata_seq') NOT NULL,
    name varchar(16) NOT NULL,
    created timestamp DEFAULT now() NOT NULL,
    last_played timestamp NOT NULL,
    played int DEFAULT 0 NOT NULL,
    site varchar(128) NOT NULL,
    wiz_invis smallint DEFAULT 0 NOT NULL,
    email varchar(64),
    trust smallint DEFAULT 0 NOT NULL,
    bank bigint DEFAULT 0 NOT NULL,
    title varchar(32) DEFAULT '' NOT NULL,
    security smallint DEFAULT 0 NOT NULL,
    act bigint DEFAULT 0 NOT NULL,
    st_bar smallint[] DEFAULT '{0,0,0}' NOT NULL,
    outcast_time bigint DEFAULT 0 NOT NULL,
    restore_time bigint DEFAULT 0 NOT NULL,
    password varchar(89) NOT NULL,
    bamfin varchar(128) DEFAULT '' NOT NULL,
    bamfout varchar(128) DEFAULT '' NOT NULL,
    rank varchar(16) DEFAULT '' NOT NULL,
    authed_by varchar(16) DEFAULT '' NOT NULL,
    backed_up timestamp,
    active boolean DEFAULT false NOT NULL
);
CREATE INDEX pdata_name_index ON pdata USING btree (lower((name)));
CREATE INDEX pdata_password_index ON pdata USING btree (password);

CREATE SEQUENCE quest_done_pchar_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;

CREATE TABLE quest_done_pchar (
    id int DEFAULT nextval('quest_done_pchar_seq'),
    quest_id int NOT NULL,
    pchar_id int NOT NULL
);

CREATE SEQUENCE skill_pchar_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;

CREATE TABLE skill_pchar (
    id int DEFAULT nextval('skill_pchar_seq') NOT NULL
)
INHERITS (generic_skill_pchar);

CREATE SEQUENCE weapon_pchar_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;

CREATE TABLE weapon_pchar (
    id int DEFAULT nextval('weapon_pchar_seq') NOT NULL
)
INHERITS (generic_skill_pchar);



ALTER TABLE ONLY affect_data_item_home
    ADD CONSTRAINT affect_data_item_home_id_key UNIQUE (id);


--
-- Name: affect_data_item_home affect_data_item_home_pkey; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY affect_data_item_home
    ADD CONSTRAINT affect_data_item_home_pkey PRIMARY KEY (id, item_id);


--
-- Name: affect_data_item_pchar affect_data_item_pchar_id_key; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY affect_data_item_pchar
    ADD CONSTRAINT affect_data_item_pchar_id_key UNIQUE (id);


--
-- Name: affect_data_item_pchar affect_data_item_pchar_pkey; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY affect_data_item_pchar
    ADD CONSTRAINT affect_data_item_pchar_pkey PRIMARY KEY (id, item_id);


--
-- Name: affect_data_pchar affect_data_pchar_id_key; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY affect_data_pchar
    ADD CONSTRAINT affect_data_pchar_id_key UNIQUE (id);


--
-- Name: affect_data_pchar affect_data_pchar_pkey; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY affect_data_pchar
    ADD CONSTRAINT affect_data_pchar_pkey PRIMARY KEY (id, pchar_id);


--
-- Name: affect_item_home affect_item_home_id_key; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY affect_item_home
    ADD CONSTRAINT affect_item_home_id_key UNIQUE (id);


--
-- Name: affect_item_home affect_item_home_pkey; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY affect_item_home
    ADD CONSTRAINT affect_item_home_pkey PRIMARY KEY (id, item_id);


--
-- Name: affect_item_pchar affect_item_pchar_id_key; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY affect_item_pchar
    ADD CONSTRAINT affect_item_pchar_id_key UNIQUE (id);


--
-- Name: affect_item_pchar affect_item_pchar_pkey; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY affect_item_pchar
    ADD CONSTRAINT affect_item_pchar_pkey PRIMARY KEY (id, item_id);


--
-- Name: affect_pchar affect_pchar_id_key; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY affect_pchar
    ADD CONSTRAINT affect_pchar_id_key UNIQUE (id);


--
-- Name: affect_pchar affect_pchar_pkey; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY affect_pchar
    ADD CONSTRAINT affect_pchar_pkey PRIMARY KEY (id, pchar_id);


--
-- Name: bestow_pdata bestow_pdata_pkey; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY bestow_pdata
    ADD CONSTRAINT bestow_pdata_pkey PRIMARY KEY (id);


--
-- Name: bounty bounty_pkey; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY bounty
    ADD CONSTRAINT bounty_pkey PRIMARY KEY (id);


--
-- Name: extradescription_item_home extradescription_item_home_id_key; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY extradescription_item_home
    ADD CONSTRAINT extradescription_item_home_id_key UNIQUE (id);


--
-- Name: extradescription_item_home extradescription_item_home_pkey; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY extradescription_item_home
    ADD CONSTRAINT extradescription_item_home_pkey PRIMARY KEY (id, item_id);


--
-- Name: extradescription_item_pchar extradescription_item_pchar_id_key; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY extradescription_item_pchar
    ADD CONSTRAINT extradescription_item_pchar_id_key UNIQUE (id);


--
-- Name: extradescription_item_pchar extradescription_item_pchar_pkey; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY extradescription_item_pchar
    ADD CONSTRAINT extradescription_item_pchar_pkey PRIMARY KEY (id, item_id);


--
-- Name: fake_pchar fake_pchar_pkey; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY fake_pchar
    ADD CONSTRAINT fake_pchar_pkey PRIMARY KEY (id);


--
-- Name: fevent_pchar fevent_pchar_pkey; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY fevent_pchar
    ADD CONSTRAINT fevent_pchar_pkey PRIMARY KEY (id);


--
-- Name: forbid_pdata forbid_pdata_pkey; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY forbid_pdata
    ADD CONSTRAINT forbid_pdata_pkey PRIMARY KEY (id);


--
-- Name: force_pchar force_pchar_id_key; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY force_pchar
    ADD CONSTRAINT force_pchar_id_key UNIQUE (id);


--
-- Name: force_pchar force_pchar_name_key; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY force_pchar
    ADD CONSTRAINT force_pchar_name_key UNIQUE (name, pchar_id);


--
-- Name: force_pchar force_pchar_pkey; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY force_pchar
    ADD CONSTRAINT force_pchar_pkey PRIMARY KEY (id, pchar_id);


--
-- Name: friend_pchar friend_pchar_id_key; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY friend_pchar
    ADD CONSTRAINT friend_pchar_id_key UNIQUE (id);


--
-- Name: friend_pchar friend_pchar_pkey; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY friend_pchar
    ADD CONSTRAINT friend_pchar_pkey PRIMARY KEY (friend_id, pchar_id);


--
-- Name: home_pdata home_pdata_pkey; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY home_pdata
    ADD CONSTRAINT home_pdata_pkey PRIMARY KEY (id);


--
-- Name: item_home item_home_pkey; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY item_home
    ADD CONSTRAINT item_home_pkey PRIMARY KEY (id);


--
-- Name: item_pchar item_pchar_pkey; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY item_pchar
    ADD CONSTRAINT item_pchar_pkey PRIMARY KEY (id);


--
-- Name: killtrack_pchar killtrack_pchar_id_key; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY killtrack_pchar
    ADD CONSTRAINT killtrack_pchar_id_key UNIQUE (id);


--
-- Name: killtrack_pchar killtrack_pchar_pkey; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY killtrack_pchar
    ADD CONSTRAINT killtrack_pchar_pkey PRIMARY KEY (vnum, pchar_id);


--
-- Name: language_pchar language_pchar_id_key; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY language_pchar
    ADD CONSTRAINT language_pchar_id_key UNIQUE (id);


--
-- Name: language_pchar language_pchar_name_key; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY language_pchar
    ADD CONSTRAINT language_pchar_name_key UNIQUE (name, pchar_id);


--
-- Name: language_pchar language_pchar_pkey; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY language_pchar
    ADD CONSTRAINT language_pchar_pkey PRIMARY KEY (id, pchar_id);


--
-- Name: pazaak_card pazaak_card_pkey; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY pazaak_card
    ADD CONSTRAINT pazaak_card_pkey PRIMARY KEY (id);


--
-- Name: pazaak_game pazaak_game_aggressor_id_key; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY pazaak_game
    ADD CONSTRAINT pazaak_game_aggressor_id_key UNIQUE (aggressor_id, defender_id);


--
-- Name: pazaak_game pazaak_game_pkey; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY pazaak_game
    ADD CONSTRAINT pazaak_game_pkey PRIMARY KEY (id);


--
-- Name: pazaak_player pazaak_player_name_key; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY pazaak_player
    ADD CONSTRAINT pazaak_player_name_key UNIQUE (name);


--
-- Name: pazaak_player pazaak_player_pkey; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY pazaak_player
    ADD CONSTRAINT pazaak_player_pkey PRIMARY KEY (id);


--
-- Name: pchar pchar_id_key; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY pchar
    ADD CONSTRAINT pchar_id_key UNIQUE (id);


--
-- Name: pchar pchar_pkey; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY pchar
    ADD CONSTRAINT pchar_pkey PRIMARY KEY (id, pdata_id);


--
-- Name: pdata pdata_pkey; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY pdata
    ADD CONSTRAINT pdata_pkey PRIMARY KEY (id);


--
-- Name: quest_done_pchar quest_done_pchar_id_key; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY quest_done_pchar
    ADD CONSTRAINT quest_done_pchar_id_key UNIQUE (id);


--
-- Name: quest_done_pchar quest_done_pchar_pkey; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY quest_done_pchar
    ADD CONSTRAINT quest_done_pchar_pkey PRIMARY KEY (quest_id, pchar_id);


--
-- Name: skill_pchar skill_pchar_id_key; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY skill_pchar
    ADD CONSTRAINT skill_pchar_id_key UNIQUE (id);


--
-- Name: skill_pchar skill_pchar_name_key; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY skill_pchar
    ADD CONSTRAINT skill_pchar_name_key UNIQUE (name, pchar_id);


--
-- Name: skill_pchar skill_pchar_pkey; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY skill_pchar
    ADD CONSTRAINT skill_pchar_pkey PRIMARY KEY (id, pchar_id);


--
-- Name: weapon_pchar weapon_pchar_id_key; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY weapon_pchar
    ADD CONSTRAINT weapon_pchar_id_key UNIQUE (id);


--
-- Name: weapon_pchar weapon_pchar_name_key; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY weapon_pchar
    ADD CONSTRAINT weapon_pchar_name_key UNIQUE (name, pchar_id);


--
-- Name: weapon_pchar weapon_pchar_pkey; Type: CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY weapon_pchar
    ADD CONSTRAINT weapon_pchar_pkey PRIMARY KEY (id, pchar_id);

--
-- Name: pazaak_game pazaak_game_finished; Type: TRIGGER; Schema: public; Owner: mud
--

CREATE TRIGGER pazaak_game_finished AFTER UPDATE ON pazaak_game FOR EACH ROW EXECUTE FUNCTION pazaak_finalize_game();


--
-- Name: ability_pchar ability_pchar_pchar_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY ability_pchar
    ADD CONSTRAINT ability_pchar_pchar_id_fkey FOREIGN KEY (pchar_id) REFERENCES pchar(id) ON DELETE CASCADE;


--
-- Name: affect_data_item_home affect_data_item_home_item_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY affect_data_item_home
    ADD CONSTRAINT affect_data_item_home_item_id_fkey FOREIGN KEY (item_id) REFERENCES item_home(id) ON DELETE CASCADE;


--
-- Name: affect_data_item_pchar affect_data_item_pchar_item_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY affect_data_item_pchar
    ADD CONSTRAINT affect_data_item_pchar_item_id_fkey FOREIGN KEY (item_id) REFERENCES item_pchar(id) ON DELETE CASCADE;


--
-- Name: affect_data_pchar affect_data_pchar_pchar_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY affect_data_pchar
    ADD CONSTRAINT affect_data_pchar_pchar_id_fkey FOREIGN KEY (pchar_id) REFERENCES pchar(id) ON DELETE CASCADE;


--
-- Name: affect_item_home affect_item_home_item_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY affect_item_home
    ADD CONSTRAINT affect_item_home_item_id_fkey FOREIGN KEY (item_id) REFERENCES item_home(id) ON DELETE CASCADE;


--
-- Name: affect_item_pchar affect_item_pchar_item_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY affect_item_pchar
    ADD CONSTRAINT affect_item_pchar_item_id_fkey FOREIGN KEY (item_id) REFERENCES item_pchar(id) ON DELETE CASCADE;


--
-- Name: affect_pchar affect_pchar_pchar_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY affect_pchar
    ADD CONSTRAINT affect_pchar_pchar_id_fkey FOREIGN KEY (pchar_id) REFERENCES pchar(id) ON DELETE CASCADE;


--
-- Name: bestow_pdata bestow_pdata_pdata_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY bestow_pdata
    ADD CONSTRAINT bestow_pdata_pdata_id_fkey FOREIGN KEY (pdata_id) REFERENCES pdata(id) ON DELETE CASCADE;


--
-- Name: extradescription_item_home extradescription_item_home_item_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY extradescription_item_home
    ADD CONSTRAINT extradescription_item_home_item_id_fkey FOREIGN KEY (item_id) REFERENCES item_home(id) ON DELETE CASCADE;


--
-- Name: extradescription_item_pchar extradescription_item_pchar_item_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY extradescription_item_pchar
    ADD CONSTRAINT extradescription_item_pchar_item_id_fkey FOREIGN KEY (item_id) REFERENCES item_pchar(id) ON DELETE CASCADE;


--
-- Name: fevent_pchar fevent_pchar_pchar_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY fevent_pchar
    ADD CONSTRAINT fevent_pchar_pchar_id_fkey FOREIGN KEY (pchar_id) REFERENCES pchar(id) ON DELETE CASCADE;


--
-- Name: forbid_pdata forbid_pdata_pdata_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY forbid_pdata
    ADD CONSTRAINT forbid_pdata_pdata_id_fkey FOREIGN KEY (pdata_id) REFERENCES pdata(id) ON DELETE CASCADE;


--
-- Name: force_pchar force_pchar_pchar_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY force_pchar
    ADD CONSTRAINT force_pchar_pchar_id_fkey FOREIGN KEY (pchar_id) REFERENCES pchar(id) ON DELETE CASCADE;


--
-- Name: friend_pchar friend_pchar_friend_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY friend_pchar
    ADD CONSTRAINT friend_pchar_friend_id_fkey FOREIGN KEY (friend_id) REFERENCES pchar(id) ON DELETE CASCADE;


--
-- Name: friend_pchar friend_pchar_pchar_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY friend_pchar
    ADD CONSTRAINT friend_pchar_pchar_id_fkey FOREIGN KEY (pchar_id) REFERENCES pchar(id) ON DELETE CASCADE;


--
-- Name: home_pdata home_pdata_pdata_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY home_pdata
    ADD CONSTRAINT home_pdata_pdata_id_fkey FOREIGN KEY (pdata_id) REFERENCES pdata(id) ON DELETE CASCADE;


--
-- Name: item_home item_home_home_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY item_home
    ADD CONSTRAINT item_home_home_id_fkey FOREIGN KEY (home_id) REFERENCES home_pdata(id) ON DELETE CASCADE;


--
-- Name: item_pchar item_pchar_pchar_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY item_pchar
    ADD CONSTRAINT item_pchar_pchar_id_fkey FOREIGN KEY (pchar_id) REFERENCES pchar(id) ON DELETE CASCADE;


--
-- Name: killtrack_pchar killtrack_pchar_pchar_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY killtrack_pchar
    ADD CONSTRAINT killtrack_pchar_pchar_id_fkey FOREIGN KEY (pchar_id) REFERENCES pchar(id) ON DELETE CASCADE;


--
-- Name: language_pchar language_pchar_pchar_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY language_pchar
    ADD CONSTRAINT language_pchar_pchar_id_fkey FOREIGN KEY (pchar_id) REFERENCES pchar(id) ON DELETE CASCADE;


--
-- Name: pazaak_card pazaak_card_player_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY pazaak_card
    ADD CONSTRAINT pazaak_card_player_id_fkey FOREIGN KEY (player_id) REFERENCES pazaak_player(id) ON DELETE CASCADE;


--
-- Name: pazaak_game pazaak_game_aggressor_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY pazaak_game
    ADD CONSTRAINT pazaak_game_aggressor_id_fkey FOREIGN KEY (aggressor_id) REFERENCES pazaak_player(id) ON DELETE CASCADE;


--
-- Name: pazaak_game pazaak_game_defender_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY pazaak_game
    ADD CONSTRAINT pazaak_game_defender_id_fkey FOREIGN KEY (defender_id) REFERENCES pazaak_player(id) ON DELETE CASCADE;


--
-- Name: pchar pchar_pdata_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY pchar
    ADD CONSTRAINT pchar_pdata_id_fkey FOREIGN KEY (pdata_id) REFERENCES pdata(id) ON DELETE CASCADE;


--
-- Name: quest_done_pchar quest_done_pchar_pchar_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY quest_done_pchar
    ADD CONSTRAINT quest_done_pchar_pchar_id_fkey FOREIGN KEY (pchar_id) REFERENCES pchar(id) ON DELETE CASCADE;


--
-- Name: skill_pchar skill_pchar_pchar_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY skill_pchar
    ADD CONSTRAINT skill_pchar_pchar_id_fkey FOREIGN KEY (pchar_id) REFERENCES pchar(id) ON DELETE CASCADE;


--
-- Name: weapon_pchar weapon_pchar_pchar_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: mud
--

ALTER TABLE ONLY weapon_pchar
    ADD CONSTRAINT weapon_pchar_pchar_id_fkey FOREIGN KEY (pchar_id) REFERENCES pchar(id) ON DELETE CASCADE;



